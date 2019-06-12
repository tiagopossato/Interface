#include "interface.h"
//#include <avr/pgmspace.h>

void execute(char *entrada);

void beginInterface() {
    INTERFACE.head = NULL;
    INTERFACE.isLocked = true;
    INTERFACE.tam = 0;
    INTERFACE.execute = execute;
}

uint8_t registerResource(Resource *node, const char *uri, GetValue _getValue, SetValue _setValue, uint8_t options) {
    node->getValue = _getValue;
    node->setValue = _setValue;

    node->options = options;
    node->uri = uri;

    if (INTERFACE.head == NULL) {
        INTERFACE.head = node;
        INTERFACE.tam++;
        return true;
    }

    Resource *tmp = INTERFACE.head;
    //TODO: Verificar se já tem um recurso com a mesma URI registrado
    while (tmp->prox != NULL) {
        tmp = tmp->prox;
    }
    tmp->prox = node;
    node->prox = NULL;
    INTERFACE.tam++;

    return true;
}

Resource * getResource(char *uri) {
    //Percorre toda a lista, buscando o recurso desejado
    Resource *rsc = INTERFACE.head;
    while (rsc != NULL) {
	//http://www.nongnu.org/avr-libc/user-manual/group__avr__pgmspace.html#gab0c75b8cce460448b747c29231da847c
	//The strcmp_P() function is similar to strcmp() except that s2 is pointer to a string in program space.
        if (strcmp(uri, rsc->uri) == 0) {
            break;
        }
        rsc = rsc->prox;
    }
    return rsc;
}

void getResourceValue(Resource * rsc) {
    memset(INTERFACE.outDataBuffer, '\0', MAX_DATA_LENGTH);
    memset(INTERFACE.buffer, '\0', MAX_DATA_LENGTH);
    if (rsc->getValue == NULL) return;
    rsc->getValue(INTERFACE.outDataBuffer, MAX_DATA_LENGTH);
    snprintf(INTERFACE.buffer, INPUT_STRING_LENGTH, READ_RETURN_FORMAT, rsc->uri, INTERFACE.outDataBuffer);
}

void execute(char *entrada) {
    char uri[MAX_URI_LENGTH];
    char data[MAX_DATA_LENGTH];
    memset(uri, '\0', MAX_URI_LENGTH);
    memset(data, '\0', MAX_DATA_LENGTH);

    //Chama a função definida pelo usuário para parsear a entrada 
    if (!parseInput(entrada, uri, data)) {
        sprintf(entrada, "%s", BADCOMMAND_RESPONSE);
        return;
    }
    //Limpa a variável de entrada
    memset(entrada, '\0', strlen(entrada));
    //Seta resposta padrão
    sprintf(entrada, "%s", BADCOMMAND_RESPONSE);

    //Busca o recurso pela uri
    Resource * rsc = getResource(uri);

    //Se o recurso não foi encontrado
    if (!rsc) {
        return;
    }

    if (!accessManagement(entrada, rsc, data)) {
        //Retorna a resposta do gerenciamento de acesso
        return;
    }

    //Verifica se é um comando de escrita
    // Isso acontece quando data não for nulo
    if (strlen(data) > 0) {
        if (rsc->setValue) {
            rsc->setValue(data); //TODO: verificar a resposta booleana de rsc->setValue
            sprintf(entrada, WRITE_RETURN_FORMAT, uri, data);
            return;
        }
    }
    memset(INTERFACE.outDataBuffer, '\0', MAX_DATA_LENGTH);
    if (rsc->getValue && rsc->getValue(INTERFACE.outDataBuffer, MAX_DATA_LENGTH)) {
        sprintf(entrada, READ_RETURN_FORMAT, uri, INTERFACE.outDataBuffer);
        return;
    }
    memset(entrada, '\0', INPUT_STRING_LENGTH);

    return;
}
