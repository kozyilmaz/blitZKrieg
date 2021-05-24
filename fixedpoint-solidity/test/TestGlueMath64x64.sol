// SPDX-License-Identifier: MIT
pragma solidity ^0.8.0;

import "truffle/Assert.sol";
import "truffle/DeployedAddresses.sol";
import "../contracts/GlueMath64x64.sol";

contract TestGlueMath64x64 {

	function abs(int x) public pure returns (int) {
		return x >= 0 ? x : -x;
	}

	function testPackUsingDeployedContract() public {
		GlueMath64x64 glue = GlueMath64x64(DeployedAddresses.GlueMath64x64());

		uint64 hi;
		uint64 lo;
		int128 expected;

		// -2.118018798719000910681
		hi = 18446744073709551613;
		lo = 16269681497853501440;
		expected = -39070550723275153408;
		Assert.equal(glue.pack128(hi, lo), expected, "Pack128-1 should work correctly");

		// -2.918018798719000910681
		hi = 18446744073709551613;
		lo = 1512286238885863424;
		expected = -53827945982242791424;
		Assert.equal(glue.pack128(hi, lo), expected, "Pack128-2 should work correctly");
	}

	function testMultiplicationWithNewGlueMath() public {
		GlueMath64x64 glue = new GlueMath64x64();

		int128 x = -39070550723275153408;
		int128 y = -53827945982242791424;
		int128 expected = 114008601486821147756;
		Assert.equal(glue.mul128x128(x,y), expected, "Mul128x128 should work correctly");
	}

	function testUnpackUsingDeployedContract() public {
		GlueMath64x64 glue = GlueMath64x64(DeployedAddresses.GlueMath64x64());

		uint64 hi;
		uint64 lo;
		int128 v = 114008601486821147756;

		// C code results:
		// -2.118018798719000910681 *
		// -2.918018798719000910681 =
		//  6.180418670702279966
		uint64 calculated_hi = 6;
		uint64 calculated_lo = 3328137044563838061;

		(hi, lo) = glue.unpack128(v);

		Assert.equal(hi, calculated_hi, "unpack128() - v.hi correct");
		Assert.equal(lo, calculated_lo, "unpack128() - v.lo correct");
	}
}
