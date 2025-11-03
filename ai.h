#ifndef IMOBITECH_AI_H
#define IMOBITECH_AI_H

#include <stddef.h>

// Gera texto descritivo usando Gemini se disponível; caso contrário, gera fallback local.
// Retorna o número de bytes escritos em outBuffer (sempre null-terminated quando outSize>0).
size_t aiGenerateText(const char* prompt, char* outBuffer, size_t outSize);

#endif // IMOBITECH_AI_H


