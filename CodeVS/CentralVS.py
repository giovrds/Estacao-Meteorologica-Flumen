import requests
import psycopg2
import time
from datetime import datetime 

conexao_bd = psycopg2.connect(
    database="Estacao",
    host="localhost",
    user="postgres",
    password="34241610",
    port="5432"
)
print("Status da conexão com o banco de dados:", conexao_bd.status)
cursor = conexao_bd.cursor()

esp32_ip = "192.168.4.1"

while True:
    try:
        
        response = requests.get(f"http://{esp32_ip}/dados", timeout=5)
        if response.status_code == 200:
           
            dados = response.json()  

            
            umidade_solo = dados.get("umidade_solo", 0)
            nivel_agua = dados.get("nivel_agua", 0)
            umidade_ar = dados.get("umidade_ar", 0)
            luminosidade = dados.get("luminosidade", 0)
            temperatura = dados.get("temperatura", 0)
            uv = dados.get("uv", 0)
            pressao = dados.get("pressao", 0)
            altitude = dados.get("altitude", 0)

            timestamp = datetime.now()

            print(f"Dados recebidos: {dados}")

            cursor.execute(
                """
                INSERT INTO dados_sensor (
                    timestamp, valor, umidade_solo, nivel_agua, umidade_ar, 
                    luminosidade, temperatura, uv, pressao, altitude
                ) VALUES (%s, %s, %s, %s, %s, %s, %s, %s, %s, %s)
                """,
                (timestamp, 1, umidade_solo, nivel_agua, umidade_ar, 
                 luminosidade, temperatura, uv, pressao, altitude)
            )
            conexao_bd.commit()
            print("Dados inseridos no banco de dados com sucesso.")

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
