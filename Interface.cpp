#include "Interface.h"
#include "Arduino.h"

Resource::Resource(const char *_uri,
                   bool (*_getValue)(char *buffer, uint8_t bufferSize),
                   bool (*_setValue)(char *data),
                   uint8_t _options,
                   Interface *interface)
{
    this->options = _options;
    this->uri = _uri;
    this->getValue = _getValue;
    this->setValue = _setValue;

    interface->registerResource(this);
}

void Interface::registerResource(Resource *rsc)
{
    if (this->head == NULL)
    {
        this->head = rsc;
        return;
    }
    Resource *tmp = this->head;
    //TODO: Verificar se já tem um recurso com a mesma URI registrado
    while (tmp->prox != NULL)
    {
        tmp = tmp->prox;
    }
    tmp->prox = rsc;
    rsc->prox = NULL;
}

Interface::Interface(/* args */)
{
    this->head = NULL;
    this->isLocked = true;
}

bool Interface::feedBuffer(char inChar, char endLine)
{
    // http://www.asciitable.com/
    if (inChar >= 32 && inChar <= 126)
    {
        sprintf(this->buffer, "%s%c", this->buffer, inChar);
        return false;
    }
    if (inChar == endLine || strlen(this->buffer) == INPUT_STRING_LENGTH - 1)
    {
        if (strlen(this->buffer) >= 1)
        {
            if (this->execute(this->buffer))
            {
                return true;
            }
        }
        memset(this->buffer, '\0', INPUT_STRING_LENGTH);
    }
    return false;
}

bool Interface::execute(char *entrada)
{
    char uri[MAX_URI_LENGTH];
    char data[MAX_DATA_LENGTH];
    memset(uri, '\0', MAX_URI_LENGTH);
    memset(data, '\0', MAX_DATA_LENGTH);

    //Chama a função definida pelo usuário para parsear a entrada
    if (!this->parseInput(entrada, uri, data))
    {
        sprintf(entrada, "%s", BADCOMMAND_RESPONSE);
        return true;
    }
    //Limpa a variável de entrada
    memset(entrada, '\0', strlen(entrada));
    //Seta resposta padrão
    sprintf(entrada, "%s", BADCOMMAND_RESPONSE);

    //Busca o recurso pela uri
    Resource *rsc = this->getResource(uri);
    //Se o recurso não foi encontrado
    if (!rsc)
    {
        return true;
    }

    if (!this->securityMiddleware(entrada, rsc, data))
    {
        //Retorna a resposta do gerenciamento de acesso
        return true;
    }

    //Verifica se é um comando de escrita
    //Isso acontece quando data não for nulo
    if (strlen(data) > 0)
    {
        if (rsc->setValue)
        {
            rsc->setValue(data);
            sprintf(entrada, WRITE_RETURN_FORMAT, uri, data);
            return true;
        }
    }

    memset(this->getValueBuffer, '\0', MAX_DATA_LENGTH);
    if (rsc->getValue && rsc->getValue(this->getValueBuffer, MAX_DATA_LENGTH))
    {
        sprintf(entrada, READ_RETURN_FORMAT, uri, this->getValueBuffer);
        return true;
    }

    memset(entrada, '\0', INPUT_STRING_LENGTH);
    return false;
}

Resource *Interface::getResource(char *uri)
{
    //Percorre toda a lista, buscando o recurso desejado
    Resource *rsc = this->head;
    while (rsc != NULL)
    {
//http://www.nongnu.org/avr-libc/user-manual/group__avr__pgmspace.html#gab0c75b8cce460448b747c29231da847c
//The strcmp_P() function is similar to strcmp() except that s2 is pointer to a string in program space.
#ifdef ESP32
        if (strcmp(uri, rsc->uri) == 0)
#else
        if (strcmp_P(uri, rsc->uri) == 0)
#endif
        {
            break;
        }
        rsc = rsc->prox;
    }
    return rsc;
}