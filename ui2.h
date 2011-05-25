#pragma once

#include <iostream>
#include <functional>
#include <string>
#include <sstream>

namespace ui2{
	int quert(int bar);	
	
	template <class T> std::function<bool(const T&)> check_true(){	
		return [](const T& m) { return true; };
	}
	template <class T> std::function<bool(const T&)> check_range(const T& l, const T& u){
		return [&](const T& m){return l <=m && m <= u;};
	}
	template <class T> std::function<bool(const T&)> check_larger(const T& lbound){
		return [&](const T& m){return lbound <= m;};
	}
	template <class T> std::function<bool(const T&)> check_smaller(const T& ubound){
		return [&](const T& m){return m <= ubound;};
	}
	template <class T> T readarg(std::string arg, std::string failmsg, std::function<bool(const T&)> chkfnc = check_true<T>()){
		std::stringstream ss(arg);
		T eval;
		ss >> eval;
		if(ss.eof() && !ss.bad() && !ss.fail() && chkfnc(eval)) return eval;
		else{
			std::cout << "Fehler bei der Eingabe!\n"
				<< failmsg << "\n";
			exit(1);
		}
	}
};
