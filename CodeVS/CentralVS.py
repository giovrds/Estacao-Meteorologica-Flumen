import requests
import psycopg2
import time
from datetime import datetime 
import subprocess 

#Inicia o código do ESP32 
platformio_executable = r"C:\Users\Giovanna\.platformio\penv\Scripts\pio.exe"

# Caminho do projeto PlatformIO
platformio_project_path = r"C:\Users\Giovanna\Documents\PlatformIO\Projects\Medicoes-Estacao"

print("Enviando código para o ESP32...")
subprocess.run([platformio_executable, "run", "-t", "upload"], cwd=platformio_project_path, shell=True)

print("Aguardando o ESP32 inicializar...")
time.sleep(10)

print("ESP32 pronto!")

conexao_bd = psycopg2.connect(
    database="Estacao",
    host="localhost",
    user="postgres",
    password="34241610",
    port="5432"
)
print("Status da conexão com o banco de dados:", conexao_bd.status)
cursor = conexao_bd.cursor()

esp32_ip = "192.168.22.99"

while True:
    try:
        
        response = requests.get(f"http://{esp32_ip}/dados", timeout=5)
        if response.status_code == 200:
           
            dados = response.json()  

            umidade_solo = dados.get("umidade_solo", 0)
            chuva = dados.get("nivel_agua", 0)
            umidade_ar = dados.get("umidade_ar", 0)
            luminosidade = dados.get("luminosidade", 0)
            temperatura = dados.get("temperatura", 0)
            qualidade_ar = dados.get("uv", 0)
            pressao = dados.get("pressao", 0)
            altitude = dados.get("altitude", 0)
            

            data_hora = datetime.now()#antigo era timestamp

            print(f"Dados recebidos: {dados}")
#antigo era insert into dados_sensor
            cursor.execute(
                """
                INSERT INTO medicao_medicao (
                    data_hora, temperatura, umidade_ar, umidade_solo, 
                    luminosidade, chuva, qualidade_ar, pressao_atmosferica, altitude
                ) VALUES (%s, %s, %s, %s, %s, %s, %s, %s, %s)
                """,
                (data_hora, temperatura, umidade_ar, umidade_solo, 
                 luminosidade, chuva, qualidade_ar, pressao, altitude)
            )
            conexao_bd.commit()
            print("Dados inseridos no banco de dados com sucesso.")
##linkar para que esse código inicie o do arduino
##arrumar para que colete de 10 em 10 min e faça uma média quando bater 1 hora, enviando junto com a leitura de 1 em 1 hora
        else:
            print(f"Erro na resposta do ESP32. Código HTTP: {response.status_code}")

    except requests.exceptions.ConnectTimeout:
        print("Erro: O tempo limite para conexão com o ESP32 expirou.")
    except requests.exceptions.RequestException as e:
        print(f"Erro na comunicação com o ESP32: {e}")
    except psycopg2.DatabaseError as e:
        print(f"Erro ao inserir dados no banco de dados: {e}")
        conexao_bd.rollback() 
    except Exception as e:
        print(f"Erro inesperado: {e}")

    time.sleep(2)
