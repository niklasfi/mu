#pragma once 	  	

#include "global.h"

#include <iostream>
#include <ostream>
#include <iomanip>

/* Word klasse 
	- sinn: auch wenn ich mit uints als id schnell arbeiten kann, will ich immer
		noch wissen, aus welcher sprache mein wort gerade ist.
	- jedem (echten) wort wird eine id >= 1 zugewiesen.
	- im großen und ganzen ist ein wort immer noch ein uint:
	- 0-30. bit: eigentliche wort-id in der sprache
	- 31. bit: die sprache des wortes
	- französische wörter haben bei _id das letzte bit auf 0
	- englische wörter haben bei _id das letzte bit auf 1
	- implizite conversion nach uint -> schnelles arbeiten, hashfunktion kann für
		uint benutzt werden
	- die wörter 0 und 0x80000000 sind nicht-Wörter. Diese sind wichtig, damit das
		lexicon auch bei der find-methode zurückgeben kann, dass es nichts gefunden
		hat, ohne gleich eine Exception zu werfen.
	- einfaches überprüfen auf nicht-wort mit NotAWord()
	- evtl reimplementation mit uint16 und einem bool, dann passen die trainings-
		daten sehr knapp rein, und wir brauchen wesentlich weniger speicher, aber 
		für conversion nach uint wird mehr zeit benötigt (1 addition), ein
		comparison operator würde 2 vergleiche benötigen, also wäre er schneller als
		erst nach uint zu casten und dann zu vergleichen (2 additionen, 1 vergleich)
*/

class Word{
	private:
		uint _id;
		static uint nextE;
		static uint nextF;
		static const uint idbits = 0x80000000-1;//alle bits außer das letze auf eins
		static const uint langbit = 0x80000000; //letztes bit auf eins
		explicit Word(uint);
	public:
		enum Language {french = 0, english = 0x80000000};
		Word(Language);
		
		uint id() const;
		Language language() const;
	
		operator uint() const;
		
		bool NotAWord() const;
		
		std::ostream& operator<<(std::ostream& os);
		
		friend class Lexicon;
};
