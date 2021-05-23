pragma solidity ^0.8.0;

import "truffle/Assert.sol";
import "truffle/DeployedAddresses.sol";
import "../contracts/GlueMath64x64.sol";

contract TestGlueMath64x64 {

	function testPack1UsingDeployedContract() public {
		GlueMath64x64 glue = GlueMath64x64(DeployedAddresses.GlueMath64x64());

		int128 expected = 7;
		uint64 hi = 18446744073709551613;
		uint64 lo = 16269681497853501440;

		Assert.equal(glue.pack128(hi, lo), expected, "Pack128-1 should work correctly");
	}

	function testPack2UsingDeployedContract() public {
		GlueMath64x64 glue = GlueMath64x64(DeployedAddresses.GlueMath64x64());

		int128 expected = 7;
		uint64 hi = 18446744073709551613;
		uint64 lo = 1512286238885863424;

		Assert.equal(glue.pack128(hi, lo), expected, "Pack128-2 should work correctly");
	}

	function testInitialBalanceWithNewMetaCoin() public {
		GlueMath64x64 glue = new GlueMath64x64();

		int128 expected = 10000;
		int128 x = -39070550723275153408;
		int128 y = -53827945982242791424;

		Assert.equal(glue.mul128x128(x,y), expected, "Mul128x128 should work correctly");
	}
}
