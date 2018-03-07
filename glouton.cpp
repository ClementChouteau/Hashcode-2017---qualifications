#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

#define Y0(tab) (tab[0])
#define X0(tab) (tab[1])
#define Y1(tab) (tab[2])
#define X1(tab) (tab[3])
#define START(tab) (tab[4])
#define FINISH(tab) (tab[5])

#define NEW_RIDE (vector< long >(6))

using namespace std;
using ride_t = std::vector< long >; // great idea

long R, C, F, N, B, T;

inline long distance(long x0, long y0, long x1, long y1)
{
	return abs(x0 - x1) + abs(y0 - y1);
}

inline long distance(const ride_t& r)
{
	return abs(X0(r) - X1(r)) + abs(Y0(r) - Y1(r));
}

double score(const vector< long >& rides_voiture, const vector< ride_t >& rides,
			 long r_next)
{
	// On simule pour savoir ou se trouve la voiture après ses rides
	// Si l'ordre des rides est fixé, il n'y a qu'une seule issue
	long x = 0;
	long y = 0;
	long t = 0;

	for (long r : rides_voiture)
	{
		const ride_t ride = rides[r];

		const long d1 = distance(x, y, X0(ride), Y0(ride));
		const long d2 = distance(X0(ride), Y0(ride), X1(ride), Y1(ride));

		const long tstart = max(t + d1, START(ride));

		t = tstart + d2;
		x = X1(ride);
		y = Y1(ride);
	}

	// ON PEUT MEMORISER CES VALEURS

	// On calcule le score
	const ride_t& ride_next = rides[r_next];
	const long	d1		= distance(x, y, X0(ride_next), Y0(ride_next));
	const long	d2 =
		distance(X0(ride_next), Y0(ride_next), X1(ride_next), Y1(ride_next));

	if (t + d1 + d2 > FINISH(ride_next)) // impossible
		return -1;

	const long b =
		(t + d1 <= START(ride_next)) ? B : 0; // bonus "pile à l'heure"

	const long score		= b + d2;
	const long tempsattente = (START(ride_next) - t + d1);
	const long tempstotal   = d1 + (tempsattente > 0 ? tempsattente : 0) + d2;

	return (double)score / (double)tempstotal;
	// return score;
}


vector< vector< long > > glouton(const vector< ride_t >& rides)
{
	vector< vector< long > > rides_per_car(
		F); // tous les rides pour chaque voiture

	// On trie les rides par date de fin de trajet
	vector< pair< long, ride_t > > rides_tries(rides.size());
	for (long r = 0; r < (long)rides.size(); r++)
		rides_tries[r] = make_pair(r, rides[r]);

	auto cmp = [](const auto& r1, const auto& r2) {
		if (FINISH(r1.second) == FINISH(r2.second))
			return START(r1.second) < START(r2.second);
		else
			return FINISH(r1.second) < FINISH(r2.second);
	};
	sort(rides_tries.begin(), rides_tries.end(), cmp);

	// On attribue les rides un par un
	for (auto ride_and_nr : rides_tries)
	{
		long r = ride_and_nr.first;

		double best_score = -1;
		long   best_car   = 0;

		// On détermine la meilleure voiture à qui le confier
		for (long car = 0; car < (long)rides_per_car.size(); car++)
		{
			double _score = score(rides_per_car[car], rides, r);
			if (_score > best_score)
			{
				best_score = _score;
				best_car   = car;
			}
		}

		// si le ride peut rapporter
		if (best_score > 0)
			rides_per_car[best_car].push_back(r);
	}

	return rides_per_car;
}


int main(int argc, char* argv[])
{
	cin >> R >> C >> F >> N >> B >> T;

	vector< ride_t > rides(N, NEW_RIDE);
	for (long k = 0; k < N; k++)
	{
		cin >> Y0(rides[k]) >> X0(rides[k]) >> Y1(rides[k]) >> X1(rides[k]) >>
			START(rides[k]) >> FINISH(rides[k]);
	}

	const vector< vector< long > > sol = glouton(rides);
	for (const auto& rs : sol)
	{
		cout << rs.size();
		for (long r : rs)
			cout << ' ' << r;

		cout << endl;
	}

	return 0;
}
