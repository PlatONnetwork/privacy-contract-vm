#include "test.h"
#include <iostream>
#include "bit.h"

/*
* test of bit, mainly on operator: & |  ^
*/

using namespace std;
using namespace platon::mpc;



TEST_F(PlatonPlainTestFixture, TestPublicPublic)
{
	Bit pa(false, PUBLIC);
	Bit pb(true, PUBLIC);

	bool pret = (pa & pb).reveal(PUBLIC);
	EXPECT_FALSE(pret);

	pret = (pa ^ pb).reveal(PUBLIC);
	EXPECT_TRUE(pret);

	pret = (pa | pb).reveal(PUBLIC);
	EXPECT_TRUE(pret);
}

TEST_F(PlatonPlainTestFixture, TestPublicBob)
{
	Bit pa(false, PUBLIC);
	Bit pb(true, BOB);

	bool pret = (pa & pb).reveal(PUBLIC);
	EXPECT_FALSE(pret);

	pret = (pa ^ pb).reveal(PUBLIC);
	EXPECT_TRUE(pret);

	pret = (pa | pb).reveal(PUBLIC);
	EXPECT_TRUE(pret);
}

TEST_F(PlatonPlainTestFixture, TestPublicAlice)
{
	Bit pa(false, PUBLIC);
	Bit pb(true, ALICE);

	bool pret = (pa & pb).reveal(PUBLIC);
	EXPECT_FALSE(pret);

	pret = (pa ^ pb).reveal(PUBLIC);
	EXPECT_TRUE(pret);

	pret = (pa | pb).reveal(PUBLIC);
	EXPECT_TRUE(pret);
}

TEST_F(PlatonPlainTestFixture, TestAliceBob)
{
	Bit pa(false, BOB);
	Bit pb(true, ALICE);

	bool pret = (pa & pb).reveal(PUBLIC);
	EXPECT_FALSE(pret);

	pret = (pa ^ pb).reveal(PUBLIC);
	EXPECT_TRUE(pret);

	pret = (pa | pb).reveal(PUBLIC);
	EXPECT_TRUE(pret);
}

TEST_F(PlatonPlainTestFixture, TestAndAllCase)
{
	bool b[] = { true, false };//values
	int p[] = { PUBLIC, ALICE, BOB };//roles

	for (int i = 0; i < 2; ++i)
		for (int j = 0; j < 3; ++j)
			for (int k = 0; k < 2; ++k)
				for (int l = 0; l < 3; ++l)
				{
					Bit b1(b[i], p[j]);
					Bit b2(b[k], p[l]);
					bool res = (b1&b2).reveal(PUBLIC);
					EXPECT_FALSE(res != (b[i] && b[k])) << "AND" << i << " " << j << " " << k << " " << l << " " << "=> " << res \
						<< "test bit error!  bit1 & bit2 should be equal with" << res;

					res = (b1 & b1).reveal(PUBLIC);
					EXPECT_FALSE(res != b[i]) << "AND " << i << " " << j << "=> " << res << ", test bit error!, bit & bit should be bit";
						
					res = (b1 & (!b1)).reveal(PUBLIC);
					EXPECT_FALSE(res) << "AND " << i << " " << j << "=> " << res << ", test bit error,  bit & (!bit) should be => false";
				}
}

TEST_F(PlatonPlainTestFixture, TestOrAllCase)
{
	bool b[] = { true, false };
	int p[] = { PUBLIC, ALICE, BOB };

	for (int i = 0; i < 2; ++i)
		for (int j = 0; j < 3; ++j)
			for (int k = 0; k < 2; ++k)
				for (int l = 0; l < 3; ++l)
				{
					Bit b1(b[i], p[j]);
					Bit b2(b[k], p[l]);
					bool res = (b1 | b2).reveal(PUBLIC);
					EXPECT_TRUE(res == (b[i] | b[k])) << "OR" << i << " " << j << " " << k << " " << l << " " << "=> " << res << ", test bit error!";

					res = (b1 | b1).reveal(PUBLIC);
					EXPECT_TRUE(res == (b[i] | b[i])) << "OR" << i << " " << j << "=> " << res << ", test bit error!";

					res = (b1 | (!b1)).reveal(PUBLIC);
					EXPECT_TRUE(res) << "OR" << i << " " << j << "=> " << res << "test bit error!";
				}
}

TEST_F(PlatonPlainTestFixture, TestXorAllCase)
{
	bool b[] = {true, false};
	int p[] = {PUBLIC, ALICE, BOB};

	for(int i = 0; i < 2; ++i)
		for(int j = 0; j < 3; ++j)
			for(int k = 0; k < 2; ++k)
				for (int l= 0; l < 3; ++l)  
				{
					Bit b1(b[i], p[j]);
					Bit b2(b[k], p[l]);
					bool res = (b1^b2).reveal(PUBLIC);
					EXPECT_FALSE(res != (b[i] ^ b[k])) << "XOR" << i << " " << j << " " << k << " " << l << " " << "=> " << res << ", test bit error!";

					res = (b1 ^ b1).reveal(PUBLIC);
					EXPECT_FALSE(res) << "XOR" << i << " " << j << "=> " << res << ", test bit error!";

					res = (b1 ^ (!b1)).reveal(PUBLIC);
					EXPECT_FALSE(!res) << "XOR" << i << " " << j << "=> " << res << "test bit error!";
				}
}
