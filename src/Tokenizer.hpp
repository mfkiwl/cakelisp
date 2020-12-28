#pragma once

#include <vector>
#include <string>

#include "Exporting.hpp"
#include "TokenEnums.hpp"

const char* tokenTypeToString(TokenType type);

struct Token
{
	TokenType type;
	// Only non-empty if type is ambiguous
	std::string contents;

	// The origin of this token, for debugging etc.
	// This is a filename for handwritten code, and something else for macro-generated tokens
	const char* source;
	// Starting at 1, because no text editor starts at "line 0"
	unsigned int lineNumber;
	// Includes quotation marks of strings. \t etc. only count as 1 column
	int columnStart;
	// Exclusive, e.g. line with "(a" would have start 0 end 1, the 'a' would have start 1 end 2
	int columnEnd;
};

void destroyToken(Token* token);

// Source should be the filename for handwritten code
// No state past a single line means this could be called in parallel
const char* tokenizeLine(const char* inputLine, const char* source, unsigned int lineNumber,
                         std::vector<Token>& tokensOut);
// Invocations of this are generated by TokenizePushGenerator()
CAKELISP_API bool tokenizeLinePrintError(const char* inputLine, const char* source,
                                         unsigned int lineNumber, std::vector<Token>& tokensOut);

bool validateParentheses(const std::vector<Token>& tokens);

void printTokens(const std::vector<Token>& tokens);
void prettyPrintTokens(const std::vector<Token>& tokens);
void prettyPrintTokensToFile(FILE* file, const std::vector<Token>& tokens);

bool writeCharToBufferErrorToken(char c, char** at, char* bufferStart, int bufferSize,
                                 const Token& token);
bool writeStringToBufferErrorToken(const char* str, char** at, char* bufferStart, int bufferSize,
                                   const Token& token);
bool appendTokenToString(const Token& token, char** at, char* bufferStart, int bufferSize);

extern CAKELISP_API int g_totalLinesTokenized;
