#ifndef __INTERFACE__H
#define __INTERFACE__H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define INPUT_STRING_LENGTH 64
#define MAX_URI_LENGTH 15  // 65535/255/65535
#define MAX_DATA_LENGTH 22 //Máximo a ser armazenado será o número de série com a resposta "SC03.01.20190527.01"

#define READ_RETURN_FORMAT "%s/%s"
#define MSG "READ_RETURN_FORMAT: " READ_RETURN_FORMAT
#pragma message(MSG)

#define WRITE_RETURN_FORMAT "%s/{\"ok\":%s}"
#define MSG2 "WRITE_RETURN_FORMAT: " WRITE_RETURN_FORMAT
#pragma message(MSG2)

#define BADCOMMAND_RESPONSE "65535/0/65531"
#define MSG3 "BADCOMMAND_RESPONSE: " BADCOMMAND_RESPONSE
#pragma message(MSG3)

// https://stackoverflow.com/questions/31596341/how-to-check-type-of-structure-in-c
// https://pt.stackoverflow.com/questions/46668/oque-s%C3%A3o-unions-porque-utiliz%C3%A1-los-dentro-de-structs
// callback é um ponteiro para funções que recebem Node*
// https://pt.stackoverflow.com/questions/2983/como-passar-uma-fun%C3%A7%C3%A3o-como-par%C3%A2metro-em-c

class Interface;

class Resource
{
private:
public:
    const char *uri;
    uint8_t options;

    /**
 * @brief Construct a new Resource object
 * 
 * @param uri 
 * @param _getValue 
 * @param _setValue 
 * @param options 
 * @param interface 
 */
    Resource(const char *uri,
             bool (*_getValue)(char *buffer, uint8_t bufferSize),
             bool (*_setValue)(char *data),
             uint8_t options,
             Interface *interface);

    /**
     * @brief 
     * 
     */
    bool (*getValue)(char *buffer, uint8_t bufferSize);

    /**
     * @brief 
     * 
     */
    bool (*setValue)(char *data);

    Resource *prox;
};

class Interface
{
private:
    bool securityMiddleware(char *bufferResposta, Resource *rsc, char *data);
    bool parseInput(char *entrada, char *uri, char *data);

    Resource *head;
    Resource *getResource(char *uri);

public:
    Interface();
    uint8_t isLocked;
    char buffer[INPUT_STRING_LENGTH];
    char getValueBuffer[MAX_DATA_LENGTH];

    /**
     * @brief Alimenta o buffer da interface
     * 
     * @param endLine 
     * @return true 
     * @return false 
     */
    bool feedBuffer(char, char endLine);
    /**
     * @brief Executa um comando recebido via porta serial
     * 
     * @return true Responder
     * @return false Não responder
     */
    bool execute(char *);
    bool execute(){return this->execute(this->buffer);};
    void registerResource(Resource *rsc);
};

#endif