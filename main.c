// ============================================================
// PROYECTO MÓDULO 3: ACELERADOR DE FUNCIONES
// Problema: Búsqueda del Máximo en un Arreglo
// ============================================================
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define N 1000000000 // 10 millones de elementos
// ------------------------------------------------------------
// VERSIÓN EN C PURO (NO MODIFICAR)
// ------------------------------------------------------------
long long maximo_c(long long *arr, int n) {
    long long max = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }   
    return max;
}
// ------------------------------------------------------------
// VERSIÓN EN ENSAMBLADOR INLINE (IMPLEMENTAR)
// ------------------------------------------------------------
//
// TODO: Implementar esta función en ensamblador inline.
//
// ENTRADA: arr = puntero al arreglo, n = número de elementos
// SALIDA: el valor máximo del arreglo
//
// INSTRUCCIONES SUGERIDAS (investiga cómo funcionan):
// - MOV : mover datos
// - CMP : comparar
// - JG : salto si es mayor (signed)
// - CMOVG: conditional move (mueve si es mayor)
// - LOOP : bucle con contador
//
// PISTA: El bucle en C hace lo siguiente:
// 1. Toma el primer elemento como máximo
// 2. Recorre el arreglo desde el segundo elemento
// 3. Si el elemento actual es mayor que el máximo, actualiza el máximo
// 4. Repite hasta el final
//
// En ASM, puedes hacer lo mismo con CMP y saltos condicionales.
// INVESTIGA: ¿Qué hace CMOVG? ¿Es más rápido que un salto?
//
// ------------------------------------------------------------
long long maximo_asm(long long *arr, int n) {
long long max = 0;
asm volatile (
    "mov (%1), %%rax \n\t" // mov (rax) = arr[0]
    "mov %2, %%rcx \n\t" // rcx = n (nuestro contador)
    "dec %%rcx \n\t"               // Restamos 1 porque ya evaluamos arr[0]
    "jz 2f \n\t"                   // Si n es igual a 1 saltamos al final
        
    "mov $1, %%r10 \n\t"           // indice i = 1

    "1: \n\t"                      // 1: inicio del bucle
    "mov (%1, %%r10, 8), %%r8 \n\t" 
        
    "cmp %%rax, %%r8 \n\t"         // Compara arr[i] (r8) con max actual [rax]
    "cmovg %%r8, %%rax \n\t"       // si r8 > rax entonces lo reemplaza 
        
    "inc %%r10 \n\t"               // Incrementa el indice
    "loop 1b \n\t"                 // Decrementa rcx, si rcx != 0 salta atras a la etiqueta 1

    "2: \n\t"                      // 2: fin
    "mov %%rax, %0 \n\t"           // mueve el max encontrado a la variable de salida "%0"
    : "=r" (max) // Salida: max
    : "r" (arr), "r" ((long long)n) // Entradas: arr, n
    : "rax", "rcx", "r8", "r10", "cc", "memory"
    );
    return max;
}
// ------------------------------------------------------------
// FUNCIÓN PRINCIPAL (NO MODIFICAR)
// ------------------------------------------------------------
int main() {
    srand(42);
    long long *arr = (long long*)malloc(N * sizeof(long long));
    for (int i = 0; i < N; i++) {
        arr[i] = rand() % 1000000;
    }
    clock_t inicio = clock();
    long long max_c = maximo_c(arr, N);
    clock_t fin = clock();
    double tiempo_c = (double)(fin - inicio) / CLOCKS_PER_SEC;
    inicio = clock();
    long long max_asm = maximo_asm(arr, N);
    fin = clock();
    double tiempo_asm = (double)(fin - inicio) / CLOCKS_PER_SEC;
    printf("Máximo en C: %lld\n", max_c);
    printf("Máximo en ASM: %lld\n", max_asm);
    if (max_c == max_asm) {
        printf("✅ Los resultados coinciden.\n");
    } else {
        printf("❌ ERROR: Los resultados NO coinciden.\n");
    }
    printf("\nTiempo en C: %.4f s\n", tiempo_c);
    printf("Tiempo en ASM: %.4f s\n", tiempo_asm);
    printf("Mejora: %.2fx\n", tiempo_c / tiempo_asm);
    free(arr);
    return 0;
}
