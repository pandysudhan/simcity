#pragma once

class Product {
private:
	bool is_soldable;
public:
	Product() { is_soldable = true; }

	bool sell() {
		if (!isSoldable())return false;

		is_soldable = false;
		return true;
	}

	bool isSoldable() { return is_soldable; }
};


