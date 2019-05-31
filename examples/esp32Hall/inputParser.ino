/**
 * Busca um caracter
 */
short buscaCaracter(char *entrada, char caracter) {
  unsigned char pos = 0;
  for (; pos <= strlen(entrada); pos++) {
    if (caracter == entrada[pos]) {
      return pos;
    }
  }
  return -1;
}

/**
 * Sanitiza string, deixando somente o que está dentro dos caracteres limitadores '{', '}'
 */
uint8_t sanitizaEntrada(char *entrada) {
  short inicio;
  short fim;
  char auxiliar[INPUT_STRING_LENGTH];
  memset(auxiliar, '\0', INPUT_STRING_LENGTH);

  inicio = buscaCaracter(entrada, '{');
  if (inicio == -1) {
    return false;
  }
  fim = buscaCaracter(entrada, '}');
  if (fim == -1) {
    return false;
  }

  memcpy(auxiliar, &entrada[inicio + 1], fim - (inicio + 1));
  sprintf(entrada, "%s", auxiliar);

  return true;
}

/**
 * Recebe uma string de entrada, analisa e retira uri e dados
 */
bool parseInput(char *entrada, char *uri, char *data) {
  uint8_t dataLength = 0;
  uint8_t fimUri;
  bool ok = false;

//  if (!sanitizaEntrada(entrada))
//    return false;


  for (fimUri = 0; fimUri < strlen(entrada); fimUri++) {
    if (entrada[fimUri] == ':') {
      break;
    }
  }

  if ((strlen(entrada) - fimUri) <= MAX_DATA_LENGTH) {
    dataLength = strlen(entrada) - fimUri;
  } else {
    dataLength = MAX_DATA_LENGTH;
  }

  memcpy(uri, entrada, fimUri);
  if (dataLength > 0) {
    memcpy(data, entrada + fimUri + 1, dataLength - 1);
  }
  return true;
}
