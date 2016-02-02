#pragma once


#include "Common.h"


/*
Na ezt siker�lt �sszehozni vertex format n�v alatt.

A l�nyeg r�viden:
Meg kell mondani, hogy a bindelt vertex streamekben hol tal�lhat� a shader bemenete.
Minden shader bemenetet egy 'InputElement' �r le. Rem�lem a forr�s alapj�n ez vil�gos.
A 'type'-ot �s 'num_components'-et alak�tsd �t, ahogy j�l esik.
A teljes shader bemnent�t egy InputElement[] array �rja le.
Az array alapj�n a Gapi l�trehoz egy 'IInputLayout*'-ot, amiben optimaliz�lt, nat�v form�ban
t�rolja a Gapi a dolgokat. Ut�na ezt az InputLayoutot lehet bindelni a Gapihoz a VertexBufferek
�s a ShaderProgram mellett.

Ha nem megfelel� InputLayout �s ShaderProgram ker�l egyidej� bindel�sre:
Leszarjuk, nem rajzolunnk, majd r�j�n a programoz�, hogy faszs�ggal eteti a Gapit.
*/


struct rInputElement 
{
	char name[65]; ///< variable name (or semantic) in shader
	eVertexAttribType type; ///< \see eVertexAttribType
	u32 num_components; ///< vector's dimension
	u32 offset; ///< this element begins $ bytes offseted from start of vertex
	u32 stream_index; ///< in which vertex stream

	/// Helper for Setting name.
	void SetName(const char* str) { // just to make life easier
		int i = 0;
		while (str[i] != '\0' && i<65) {
			name[i] = str[i];
			i++;
		}
		name[i] = '\0';
	}
};


class IInputLayout 
{
public:
	/// Get the number of vertex elements.
	virtual size_t GetNumElements() = 0;
	/// Get the indexth vertex element.
	virtual const rInputElement& GetElement(size_t index) = 0;

	virtual void Release() = 0;
};