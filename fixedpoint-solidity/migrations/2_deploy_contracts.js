const ABDKMath64x64 = artifacts.require("ABDKMath64x64");
const GlueMath64x64 = artifacts.require("GlueMath64x64");

module.exports = function(deployer) {
  deployer.deploy(ABDKMath64x64);
  deployer.link(ABDKMath64x64, GlueMath64x64);
  deployer.deploy(GlueMath64x64);
};
