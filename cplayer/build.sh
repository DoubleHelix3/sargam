#!/bin/bash

clang code/*.c -lraylib -lm

if [ $? -eq 0 ] 
then
  ./a.out
fi

