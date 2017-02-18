#ifndef LISTA_COMANDO_H
#define LISTA_COMANDO_H

struct Comando{
  boolean motores[24];
  unsigned int inicio;
  unsigned int fim;
  int intensidade;
  struct Comando *prox;
};

struct Lista{
  struct Comando *comando;
  int quantidade;
};

void criar(Lista *l){

  l->comando = NULL;
  l->quantidade = 0;
}

boolean inserir(Lista *l, boolean *motores, unsigned int inicio, unsigned int fim, byte intensidade){

  if(l->quantidade == 0){
    Comando *novo_comando;
    novo_comando = (Comando*)malloc(sizeof(Comando));

    if(novo_comando == NULL) return false;

    for(int i=0; i<24; i++) novo_comando->motores[i] = motores[i];
    novo_comando->inicio = inicio;
    novo_comando->fim = fim;
    novo_comando->intensidade = intensidade;
    novo_comando->prox = NULL;
    
    l->comando = novo_comando;
    l->quantidade++;
  }else{
    
    Comando *novo_comando;
    novo_comando = (Comando*)malloc(sizeof(Comando));

    if(novo_comando == NULL) return false;

    for(int i=0; i<24; i++) novo_comando->motores[i] = motores[i];
    novo_comando->inicio = inicio;
    novo_comando->fim = fim;
    novo_comando->intensidade = intensidade;

    Comando *ultimo = l->comando;
    while(ultimo->prox != NULL) ultimo = ultimo->prox;

    ultimo->prox = novo_comando;
    novo_comando->prox = NULL;
    l->quantidade++;
  }

  return true;
}

void removerAll(Lista *lista){

  Comando *aux = lista->comando;
  Comando *aux2;
  while(aux != NULL){

    aux2 = aux;
    aux = aux->prox;
    free(aux2);
  }

  lista->comando = NULL;
  lista->quantidade = 0;
}

#endif
