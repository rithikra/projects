#include "searcheng.h"
#include "combiners.h"
WebPageSet ANDWebPageSetCombiner::combine(const WebPageSet& setA, const WebPageSet& setB){
	WebPageSet AND;
	std::set<WebPage*>::const_iterator a;
	for (a = setA.begin(); a != setA.end(); ++a){
			if (setB.find(*a) != setB.end()){
				AND.insert(*a);
			}
	}
	return AND;
}
WebPageSet ORWebPageSetCombiner::combine(const WebPageSet& setA, const WebPageSet& setB){
	WebPageSet OR = setB;
	std::set<WebPage*>::const_iterator a; 
	for (a = setA.begin(); a != setA.end(); ++a){
			if (setB.find(*a) == setB.end()){
				OR.insert(*a);
			}
	}
	return OR;
}


WebPageSet DiffWebPageSetCombiner::combine(const WebPageSet& setA, const WebPageSet& setB){
	WebPageSet DIFF = setA;
	std::set<WebPage*>::const_iterator a; 
	for (a = setA.begin(); a != setA.end(); ++a){
			if (setB.find(*a) != setB.end()){
				DIFF.erase(*a);
			}
	}
	return DIFF;
}


