#!/bin/bash
cc -std=c99 -Wall evaluation.c mpc.c -ledit -lm -o evaluation
