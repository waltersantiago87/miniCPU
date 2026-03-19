# Mini CPU — Contagem de Números Pares

Este projeto implementa uma **mini CPU simulada em C**, capaz de executar instruções básicas (MOV, ADD, SUB, JMP, etc.) e manipular memória e registradores.

---

## Objetivo

Contar quantos números **pares** existem em um conjunto de valores armazenados na memória da CPU.

---

## Funcionamento

- São pré-carregados **8 valores** nos endereços `0x10` a `0x17`:



- Cada valor é analisado para verificar se é **par**, utilizando **subtrações sucessivas de 2** (sem uso do operador `%`).

### Regra de paridade:
- Resultado final `0` → número **par**
- Resultado final `1` → número **ímpar**

---

## Resultado

A quantidade de números pares é armazenada no endereço:

## Integrantes - Grupo 01

Dávila Peixoto
João Carlos Vasconcelos
João Pedro Pessôa
Walter Maia
