# k-means
O projeto foi todo desenvolvido e testado utilizando o sistema operacional __Ubuntu__, portanto deveria funcionar normalmente em qualquer SO.

# Dependências
- lib-boost
- make
- g++
- lib-pthread
   
Para instalação da `lib-boost` em sistemas operacionais linux basta executar o comando
```
sudo apt-get install libboost-all-dev
```
Para instalação no Windows os arquivos binários podem ser baixados em
```
https://sourceforge.net/projects/boost/files/boost-binaries/1.55.0/
```

# Execução

- Para execução basta compilar o projeto utilizando
```
make all
```
- Dentro da pasta `k-means/` devem existir outras duas pastas:
    - `bases` (onde devem estar as bases disponibilizadas)
    - `results` (onde será escrito o resultado da execução)

Para rodar basta chamar o programa desejado:
- Para versão sequencial `./k-means-seq`, passando como parâmetro a base desejada, como por exemplo para a execução da base `int_base_59.data`:
```
./k-means-seq 59
```
- Para execução da versão paralela do algoritmo deve-se seguir as mesmas instruções da versão sequêncial porém depois do parâmetro da base a ser utilizada deve-se informar o número de _threads_, por exemplo para executar a base `int_base_59.data` utilizando 4 _threads_:
```
./k-means-par 59 4
```