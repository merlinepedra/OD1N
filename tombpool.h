/*

What this it ?
Just another thread pool simple api to work with double linked queue

Author:Antonio Costa aka " Cooler_ "
contact: c00f3r[at]gmail[dot]com
contribuitor: MarceloMF - Marcelo M. Fleury
email: marcelo.fleury@4linux.com.br

    Copyright (C) 2012 tombpool authors,
    
    This file is part of tombpool
    
    tombpool is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    tombpool is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>


/* Individual job */
typedef struct piscina_tarefa{
 void* arg;                    // argumentos da função
 void* (*function)(void* arg);  // ponteiro para função
 struct piscina_tarefa* proximo;   // proxima tarefa              
 struct piscina_tarefa* anterior;  //tarefa anterior          
}piscina_tarefa;

typedef struct piscina_fila{
 int NumTarefa;         // tarefas na fila
 sem_t *filaSemaforo;  // semaforo para controlar as tarefas
 piscina_tarefa *start;  // inicio da fila
 piscina_tarefa *end; // final da fila
}piscina_fila;

// struct da thread pool
typedef struct piscina{
 int NumThread;   // num threads
 pthread_t* threads;              
 piscina_fila* TarefaFila; // ponteiro apra tarefa
}piscina;

// ponte de dados
typedef struct thread_data{                            
 pthread_mutex_t *mutex_p;
 piscina        *dados;
}thread_data;


void *xmalloc(unsigned int len);

// to init api
piscina* Dig_TombPool(int NumThread);
void poolWheel(piscina* dados);

// to add task
int Add_Corpse(piscina* dados, void *(*function_p)(void*), void* arg_p);

// to execute tasks and free heap...
void Cover_TombPool(piscina* dados,int NumThead);
int piscina_fila_init(piscina* dados);
void piscina_fila_add(piscina* dados, piscina_tarefa* newjob_p);
int piscina_fila_removelast(piscina* dados);
piscina_tarefa* piscina_fila_peek(piscina* dados);

