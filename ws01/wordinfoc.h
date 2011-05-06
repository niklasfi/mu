#pragma once

typedef unsigned int uint;

#include <list>
#include <utility> //pair
#include "word.h"
#include "wordinfo.h"
#include <cmath>

typedef std::list<std::pair<double, uint>> plist; //spart Tipparbeit

class WordinfoC{
	uint singlecount; //privat, verhindert zweimaliges setzen von singlecount
	static uint maxlen;  //privat, verhintert zweimaliges setzen von maxlen
	public:
	
	plist pairs; //die Liste mit Übersetzungen in des ursprungswortes. .pairs[f].second gibt id der Übersetzung
	
	WordinfoC(); //leerer Konstruktor. Erstellt leere Liste
	WordinfoC(uint singlecount); //leere Liste, singlecount gesetzt
	WordinfoC(Wordinfo); //konvertiere eine alte Wordinfo
	
	void add_translation(Word w, double rel_freq); //fügt eine Übersetzung in der Liste sortiert (nach relfreq) hinzu. Es wird angenommen, dass das Wort noch nicht in der Liste vorhanden war.
	void set_singlecount(uint count); //setze singlecount, darf nur aufgerufen werden, wenn singlecount 0 ist.
	static void set_maxlen(uint size); //setzt die maximale Länge der Liste pairs. ist maxlen = 0, so ist die listenlänge nicht begrenzt. dauf auch nur einmal aufgerufen werden
	
	class singleCountAlreadySetException{}; //Wird geworfen, wenn set_singlecount zweimal aufgerufen wird.
	class maxlenAlreadySetException{}; //Wird geworfen, wenn set_maxlen zweimal aufgerufen wird.
};
