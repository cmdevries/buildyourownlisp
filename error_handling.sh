#!/bin/bash
cc -std=c99 -Wall error_handling.c mpc.c -ledit -lm -o error_handling
