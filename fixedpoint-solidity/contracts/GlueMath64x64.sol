// SPDX-License-Identifier: MIT
pragma solidity ^0.8.0;

import "./ABDKMath64x64.sol";

contract GlueMath64x64 {

	event ValuePacked(int128 output);
	event ValueMultiplied(int128 output);

	function pack128 (uint64 hi, uint64 lo) public returns (int128) {
		uint128 result = uint128(hi) << 64 |(lo);
		emit ValuePacked(int128(result));
		return int128(result);
	}

	function mul128x128 (int128 x, int128 y) public returns (int128) {
		int128 result = ABDKMath64x64.mul(x, y);
		emit ValueMultiplied(result);
		return result;
	}

}
