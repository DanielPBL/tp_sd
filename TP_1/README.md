
# Trabalho Prático 1: Comunicação #

Implementação de um serviço de consultas a um log distribuído.

## Compilação ##

O projeto vai acompanhado de dois arquivos `Makefile` que geram os executáveis
do servidor e do cliente (que se encontra na pasta raiz `.`) e dos testes (que se
encontra na pasta `tests`). Desta forma, para compilar os programas basta executar
o comando `make` dentro de cada um dos diretórios indicados.

## Execução ##

### Geração dos Logs ###

Dentro da pasta `tests/build` será gerado o programa `loggen.out` que é responsável
por gerar os arquivos de logs usados para os testes, sendo utilizado da seguinte
forma:
```
loggen.out ID TAMANHO
```
* ID: id do servidor
* TAMANHO: tamanho mínimo do arquivo de saída em MB

Após a execução do programa deverá existir um aquivo nomeado `maquina.ID.log` no
diretório atual.

### Servidor ###

Dentro da pasta `build`, execute o comando:
```
server.out IP PORTA_TCP PORTA_UDP
```
* IP: IP do servidor
* PORTA_TCP: porta utilizada para receber conexões dos clientes
* PORTA_UDP: porta para realização do broadcast (descoberta de rede)

*Observação:* O arquivo de log gerado para teste deve ser copiado para o diretório
do servidor.

### Cliente ###

Dentra da pasta `build`, execute o comando:
```
client.out IP PORTA_UDP (IP_SERVER PORTA_SERVER)
```
* IP: IP do cliente
* PORTA_UDP: porta que deve ser escutada para encontrar servidores (descoberta de rede)

Caso queira informar algum servidor inicialmente para o cliente, existem ainda dois
parâmetros opcionais:
* IP_SERVER _(Opcional)_: IP do servidor
* PORTA_SERVER _(Opcional)_: porta que o servidor espera receber conexões

### Unit Tests ###

Para executar os testes unitários basta acessar o diretório `tests/build` e executar
os seguintes comandos:
```
test.out
```
*Observação:* É necessário que exista um arquivo de log gerado pelo `loggen.out` no
diretório de execução.
