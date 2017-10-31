
# Trabalho Prático 2: Serviço P2P #

Implementação de um serviço P2P estruturado.

## Compilação ##

O projeto vai acompanhado de dois arquivos `Makefile` que gera o executável do
peer (que se encontra na pasta raiz `.`) e dos testes (que se encontra na pasta
 `tests`). Desta forma, para compilar os programas basta executar o comando
`make` dentro de cada um dos diretórios indicados.

## Execução ##

Dentro da pasta `build`, execute o comando:
```
peer.out IP PORTA
```
* **IP**: IP do peer
* **PORTA**: porta utilizada para receber conexões

### Comandos disponíveis ###

* **ENTER "IP" PORTA**: entra na rede P2P do peer informado
* **STORE(<K,"V">)**: armazena a tupla <K,V> no peer
* **FIND(K)**: busca a tupla identificada por K na rede
* **LIST**: lista os peers vizinhos e as tuplas armazenadas neste peer
* **HELP**: exibe o texto de ajuda
* **QUIT**: encerra a aplicação

### Unit Tests ###

Para executar os testes unitários basta acessar o diretório `tests/build` e executar
o seguinte comando:
```
test.out
```
