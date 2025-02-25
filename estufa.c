#include "estufa.h"

estufa estufas[3]={{1,true,true,false,true,30,80,22},
                   {2,false,false,false,false,40,60,25},
                   {3,true,false,false,false,60,50,27}};

void atualiza_estado_estufa(){
  for (uint8_t i = 0; i < NUM_ESTUFAS; i++)
  {
    //Umidade do solo
    if(estufas[i].umidade_solo <= MIN_UMIDADE_SOLO && !estufas[i].status_irrigacao && !estufas[i].status_fertilizante)
    {
      estufas[i].status_irrigacao=true;
    }else if(estufas[i].umidade_solo >= MAX_UMIDADE_SOLO && estufas[i].status_irrigacao)
    {
      estufas[i].status_irrigacao=false;
    }
    //Umidade do ar
    if (estufas[i].umidade_ar <= MIN_UMIDADE_AR && !estufas[i].status_umidade)
    {
      estufas[i].status_umidade=true;
    }else if (estufas[i].umidade_ar >= MAX_UMIDADE_AR && estufas[i].status_umidade)
    {
      estufas[i].status_umidade=false;
    }
    //Temperatura
    if (estufas[i].temperatura >= MAX_TEMPERATURA && !estufas[i].status_ventilacao)
    {
      estufas[i].status_ventilacao=true;
      if(estufas[i].umidade_ar < MAX_UMIDADE_AR)estufas[i].status_umidade=true;
    }else if(estufas[i].temperatura <= MIN_TEMPERATURA && estufas[i].status_ventilacao){
      estufas[i].status_ventilacao=false;
    }
  }
}

void imprime_informacoes(){
  for (uint8_t i = 0; i < NUM_ESTUFAS; i++)
  {
    printf("Estufa |%d|\n",estufas[i].id);
    printf("Umidade solo: %.1f%%\n",estufas[i].umidade_solo);
    printf((estufas[i].status_irrigacao) ? "Irrigacao: ativada\n" : "Irrigacao: desativada\n");
    printf("Umidade ar: %.1f%%\n",estufas[i].umidade_ar);
    printf((estufas[i].status_umidade) ? "Aspersor: ativado\n" : "Aspersor: desativado\n");
    printf("Temperatura: %.1fC°\n",estufas[i].temperatura);
    printf((estufas[i].status_ventilacao) ? "Ventilacao: ativada\n" : "Ventilacao: desativada\n");
    printf("***************************************\n");
    
  } 
}

void simula_clima(){
  for (uint8_t i = 0; i < NUM_ESTUFAS; i++)
  {
    if(estufas[i].status_umidade && estufas[i].umidade_ar < MAX_VALOR)
    {
      estufas[i].umidade_ar+=TAXA_UMIDADE_AR;
    }else if (estufas[i].umidade_ar > MIN_VALOR){
      estufas[i].umidade_ar-=TAXA_DEC_UMIDADE_AR;
    }
    
    if((estufas[i].status_irrigacao || estufas[i].status_fertilizante) && estufas[i].umidade_solo < MAX_VALOR)
    {
      estufas[i].umidade_solo+=TAXA_UMIDADE_SOLO;
    }else if (estufas[i].umidade_solo > MIN_VALOR){
      estufas[i].umidade_solo-=TAXA_DEC_UMIDADE_SOLO;
    }

    if(estufas[i].status_ventilacao && estufas[i].temperatura > MIN_VALOR)
    {
      estufas[i].temperatura-=TAXA_DEC_TEMPERATURA;
      if(estufas[i].umidade_ar > MIN_VALOR)estufas[i].umidade_ar-=(TAXA_DEC_UMIDADE_AR*1.5);
    }else if (estufas[i].temperatura < MAX_VALOR){
      estufas[i].temperatura+=TAXA_TEMPERATURA;
    }
  }
}

void alterna_irrigacao(estufa *p){
  if(!p->status_fertilizante){
    p->status_irrigacao=false;
    p->status_fertilizante=true;
  }else{
    p->status_fertilizante=false;
  }
}


