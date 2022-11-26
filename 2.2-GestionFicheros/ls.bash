#!/bin/bash

__PATH="/home/ab-82/Desktop/ASOR-Practicas-C/2.1-Introduccion" #$(pwd)

cd $__PATH
echo "Aprendiendo ls en $__PATH"

function mostrarComando(){
    echo $1
    $1
    echo -e "\n\n\n"
}

mostrarComando "ls -a"
mostrarComando "ls -l" 
mostrarComando "ls -d"
mostrarComando "ls -h"
mostrarComando "ls -i"
mostrarComando "ls -R"
mostrarComando "ls -1"
mostrarComando "ls -F"
mostrarComando "ls --color"

