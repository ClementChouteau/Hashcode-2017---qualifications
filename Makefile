glouton: glouton.cpp
	g++ -Wall -O2 glouton.cpp -o glouton

out: glouton
	./make_out.sh

clean:
	rm -f glouton
	rm -f out/a_example.out
	rm -f out/b_should_be_easy.out
	rm -f out/c_no_hurry.out
	rm -f out/d_metropolis.out
	rm -f out/e_high_bonus.out
