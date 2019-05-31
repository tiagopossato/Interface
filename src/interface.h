/* 
 * File:   interface.h
 * Author: tiago
 *
 * Created on 8 de Maio de 2019, 22:35
 */
#ifndef INTERFACE_H
#define INTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#define DEBUG_INTERFACE
    //----------------------------------------------------------   
#define READ_RETURN_FORMAT "%s:%s"
#define MSG "READ_RETURN_FORMAT: " READ_RETURN_FORMAT
#pragma message(MSG)

#define WRITE_RETURN_FORMAT "%s:{\"ok\":%s}"
#define MSG2 "WRITE_RETURN_FORMAT: " WRITE_RETURN_FORMAT
#pragma message(MSG2)

#define BADCOMMAND_RESPONSE "65535/0/65531"
#define MSG3 "BADCOMMAND_RESPONSE: " BADCOMMAND_RESPONSE
#pragma message(MSG3)

#define INPUT_STRING_LENGTH 40
#define MAX_URI_LENGTH 15 // 65535/255/65535
#define MAX_DATA_LENGTH 27 //Número de série "ok":"SC03.01.20190527.01"
    // https://stackoverflow.com/questions/31596341/how-to-check-type-of-structure-in-c
    // https://pt.stackoverflow.com/questions/46668/oque-s%C3%A3o-unions-porque-utiliz%C3%A1-los-dentro-de-structs

    // callback é um ponteiro para funções que recebem Node*
    //https://pt.stackoverflow.com/questions/2983/como-passar-uma-fun%C3%A7%C3%A3o-como-par%C3%A2metro-em-c

    typedef uint8_t(*SetValue)(char *);
    typedef uint8_t(*GetValue)(char *, uint8_t);

    struct sNode {
        const char *uri;
        uint8_t options; // Tipo de acesso
        SetValue setValue;
        GetValue getValue;
        struct sNode *prox;
    };
    typedef struct sNode Resource;

    struct {
        Resource *head;
        uint16_t tam;
        uint8_t isLocked;
        char buffer[INPUT_STRING_LENGTH];
        char outDataBuffer[MAX_DATA_LENGTH];
        void (*execute)(char *);
    } INTERFACE;

    void beginInterface();
    /**
       @brief Registra um item da interface na lista de itens
       @param node
       @param uri
       @return uint8_t
     */
    uint8_t registerResource(
            Resource *node,
            const char *uri,
            GetValue _getValue,
            SetValue _setValue,
            uint8_t options
            );
    Resource * getResource(char *uri);
    void getResourceValue(Resource *rsc);
    bool parseInput(char *entrada, char *uri, char *data);
    bool accessManagement(char *bufferResposta, Resource *rsc, char *data);

#ifdef __cplusplus
}
#endif

#endif /* INTERFACE_H */

