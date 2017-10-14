
# Trabalho Prático 2: Serviço P2P #

Implementação de um serviço P2P estruturado.

## Compilação ##

O projeto vai acompanhado de dois arquivos `Makefile` que gera o executável do
peer (que se encontra na pasta raiz `.`) e dos testes (que se encontra na pasta
 `tests`). Desta forma, para compilar os programas basta executar o comando
`make` dentro de cada um dos diretórios indicados.

## Execução ##

### Geração dos Logs ###

Dentro da pasta `build`, execute o comando:
```
peer.out IP IP PORTA
```
* **IP**: IP do peer
* **PORTA**: porta utilizada para receber conexões

### Cliente ###

Dentra da pasta `build`, execute o comando:
```
client.out IP PORTA_UDP (IP_SERVER PORTA_SERVER)
```
* **IP**: IP do cliente
* **PORTA_UDP**: porta que deve ser escutada para encontrar servidores (descoberta de rede)

Caso queira informar algum servidor inicialmente para o cliente, existem ainda dois
parâmetros opcionais:
* **IP_SERVER _(Opcional)_**: IP do servidor
* **PORTA_SERVER _(Opcional)_**: porta que o servidor espera receber conexões

#### Comandos disponíveis ####

* **ENTER "IP" PORTA**: entra na rede P2P do peer informado
* **STORE(<K,V>)**: armazena a tupla <K,V> no peer
* **FIND(K)**: busca a tupla identificada por K na rede
* **LIST**: lista as tuplas armazenadas no peer e o peer no qual está conectado
* **HELP**: exibe o texto de ajuda
* **QUIT**: encerra a aplicação

### Unit Tests ###

Para executar os testes unitários basta acessar o diretório `tests/build` e executar
o seguinte comando:
```
test.out
```
