#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "function_declaration.h"
#include "expression.h"
#include "var_declaration.h"
#include "parser.h"

astNode *parseReturnInstruction(TokenList *tokenList, int *currentToken, error *err) {
    assert(tokenList->tokens[*currentToken].type == TOKEN_FUNCTION_RETURN);
    ++*currentToken;

    if (*currentToken > tokenList->nb_tokens) {
        return endOfInputError(err);
    }

    if (tokenList->tokens[*currentToken].type == TOKEN_SEMICOLON) {
        ++*currentToken;
        return newEmptyReturnNode();
    }

    astNode *expression = parseExpression(tokenList, currentToken, err);
    if (err->value != ERR_SUCCESS) {
        return NULL;
    }

    if (tokenList->tokens[*currentToken].type != TOKEN_SEMICOLON) {
        err->value = ERR_SYNTAX;
        err->message = "Expected a semicolon after return statement";
        return addPositionToError(err, tokenList->tokens[*currentToken]);
    }

    ++*currentToken;
    return newReturnValueNode(expression);
}

functionParameter parseFunctionParameter(TokenList *tokenList, int *currentToken, error *err) {
    functionParameter parameter;
    initType type = parseType(tokenList, currentToken, err);

    if (err->value != ERR_SUCCESS) {
        return parameter;
    }
    if (*currentToken > tokenList->nb_tokens) {
        endOfInputError(err);
        return parameter;
    }

    if (tokenList->tokens[*currentToken].type != TOKEN_IDENTIFIER) {
        err->value = ERR_SYNTAX;
        err->message = "Expected an identifier after type";
        addPositionToError(err, tokenList->tokens[*currentToken]);
        return parameter;
    }
    char *parameterName = tokenList->tokens[*currentToken].value;
    ++*currentToken;

    parameter.type = type;
    parameter.name = strdup(parameterName);

    return parameter;
}

functionParameter *newFunctionParameters(functionParameter parameter) {
    functionParameter *parameters = malloc(sizeof(functionParameter));
    parameters[0] = parameter;
    return parameters;
}

functionParameter *addFunctionParameter(
        functionParameter *parameters, functionParameter parameter, int nbParameters) {
    parameters = realloc(parameters, (nbParameters + 1) * sizeof(functionParameter));
    parameters[nbParameters] = parameter;
    return parameters;
}

void freeFunctionParameters(functionParameter *parameters, int nbParameters) {
    for (int i = 0; i < nbParameters; ++i) {
        free(parameters[i].name);
    }
    free(parameters);
}


functionParameter *parseFunctionParameters(TokenList *tokenList, int *currentToken, int *parameterCount, error *err) {
    if (tokenList->tokens[*currentToken].type != TOKEN_LPAREN) {
        err->value = ERR_SYNTAX;
        err->message = "Expected an open parenthesis after function name";
        return addPositionToError(err, tokenList->tokens[*currentToken]);
    }
    ++*currentToken;
    if (*currentToken > tokenList->nb_tokens) {
        return endOfInputError(err);
    }


    if (tokenList->tokens[*currentToken].type == TOKEN_RPAREN) {
        ++*currentToken;
        return NULL;
    }

    functionParameter parameter = parseFunctionParameter(tokenList, currentToken, err);

    if (err->value != ERR_SUCCESS) {
        return NULL;
    }

    functionParameter *parameters = newFunctionParameters(parameter);
    int nbParameters = 1;


    while (tokenList->tokens[*currentToken].type == TOKEN_COMMA) {
        ++*currentToken;
        if (*currentToken > tokenList->nb_tokens) {
            return endOfInputError(err);
        }

        parameter = parseFunctionParameter(tokenList, currentToken, err);
        if (err->value != ERR_SUCCESS) {
            return NULL;
        }

        parameters = addFunctionParameter(parameters, parameter, nbParameters);
        ++nbParameters;
    }
    if (*currentToken > tokenList->nb_tokens) {
        return endOfInputError(err);
    }

    if (tokenList->tokens[*currentToken].type != TOKEN_RPAREN) {
        err->value = ERR_SYNTAX;
        err->message = "Expected a closing parenthesis after function parameters";
        return addPositionToError(err, tokenList->tokens[*currentToken]);
    }

    ++*currentToken;
    *parameterCount = nbParameters;
    return parameters;
}

astNode *parseFunctionDeclaration(TokenList *tokenList, int *currentToken, error *err) {
    assert(tokenList->tokens[*currentToken].type == TOKEN_FUNCTION_DECLARATION);
    ++*currentToken;

    if (*currentToken > tokenList->nb_tokens) {
        return endOfInputError(err);
    }
    if (tokenList->tokens[*currentToken].type != TOKEN_IDENTIFIER) {
        err->value = ERR_SYNTAX;
        err->message = "Expected an identifier after function keyword";
        return addPositionToError(err, tokenList->tokens[*currentToken]);
    }
    char *functionName = tokenList->tokens[*currentToken].value;

    ++*currentToken;
    if (*currentToken > tokenList->nb_tokens) {
        return endOfInputError(err);
    }

    int parameterCount;

    functionParameter *parameters = parseFunctionParameters(tokenList, currentToken, &parameterCount, err);
    if (err->value != ERR_SUCCESS) {
        return NULL;
    }

    if (*currentToken > tokenList->nb_tokens) {
        return endOfInputError(err);
    }

    initType returnType;
    int isVoid = 1;
    if (tokenList->tokens[*currentToken].type != TOKEN_LBRACE) {
        returnType = parseType(tokenList, currentToken, err);
        if (err->value != ERR_SUCCESS) {
            return NULL;
        }
        isVoid = 0;
    }

    if (*currentToken > tokenList->nb_tokens) {
        return endOfInputError(err);
    }

    if (tokenList->tokens[*currentToken].type != TOKEN_LBRACE) {
        err->value = ERR_SYNTAX;
        err->message = "Expected an opening brace after function declaration";
        return addPositionToError(err, tokenList->tokens[*currentToken]);
    }

    InstructionBlock *functionBody = parseInstructionBlockWithBraces(tokenList, currentToken, err);
    if (err->value != ERR_SUCCESS) {
        return NULL;
    }

    astNode *body = newInstructionBlockNode(functionBody);

    return newFunctionDeclarationNode(functionName, parameters, parameterCount, isVoid, returnType, body);
}
