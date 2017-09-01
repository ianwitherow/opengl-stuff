//
//  helpers.hpp
//  Moo
//
//  Created by Ian Witherow on 8/23/17.
//  Copyright © 2017 Ian Witherow. All rights reserved.
//

#ifndef helpers_hpp
#define helpers_hpp


using namespace std;

string ResourceFile(string filename) {
	#ifdef _WIN32
		return "src/" + filename;
	#endif

	return filename;
}




#endif /* helpers_hpp */
