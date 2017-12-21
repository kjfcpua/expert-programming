CXX=g++

all:
	$(CXX) -o test_pre_increment_O0.s  increment_test.cc -S -Wall -Werror -std=c++11 -DPRE_INCREMENT
	$(CXX) -o test_post_increment_O0.s increment_test.cc -S -Wall -Werror -std=c++11 -DPOST_INCREMENT
	sha1sum test_pre_increment_O0.s test_post_increment_O0.s
	$(CXX) -o test_pre_increment_O1.s  increment_test.cc -S -O1 -Wall -Werror -std=c++11 -DPRE_INCREMENT
	$(CXX) -o test_post_increment_O1.s increment_test.cc -S -O1 -Wall -Werror -std=c++11 -DPOST_INCREMENT
	sha1sum test_pre_increment_O1.s test_post_increment_O1.s
	$(CXX) -o test_pre_increment_O2.s  increment_test.cc -S -O2 -Wall -Werror -std=c++11 -DPRE_INCREMENT
	$(CXX) -o test_post_increment_O2.s increment_test.cc -S -O2 -Wall -Werror -std=c++11 -DPOST_INCREMENT
	sha1sum test_pre_increment_O2.s test_post_increment_O2.s
	$(CXX) -o test_pre_increment  increment_test.cc -std=c++11 -DPRE_INCREMENT
	$(CXX) -o test_post_increment increment_test.cc -std=c++11 -DPOST_INCREMENT

clean:
	rm -f test_p*_increment_O*.s test_p*_increment_O*.o test_p*_increment
