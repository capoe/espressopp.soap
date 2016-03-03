#ifndef _SOAP_SPECTRUM_HPP_
#define _SOAP_SPECTRUM_HPP_
#include "SystemAccess.hpp"
#include "Portal.hpp"

namespace espressopp {
    namespace soap {


class CenterDensity
{
	CenterDensity() {}
};


class TargetDensity
{
	TargetDensity() {}
};

class Center
{
	Center() {}
};

class Target
{
	Target() {}
};


class Spectrum : public SystemAccess
{
public:
	Spectrum(shared_ptr<System> system, shared_ptr<Options> options) : SystemAccess(system), _options(options) {}

	void saveAndClean() { std::cout << "spectrum::save&clean" << std::endl; }
	void save();
	void clean();

	void compute() { std::cout << "spectrum::computing" << std::endl; }
	void computePower();
	void computeLinear();

	static void registerPython() {
		using namespace espressopp::python;
		class_<Spectrum>("soap_Spectrum", init< shared_ptr<System>, shared_ptr<Options> >())
		    .def("compute", &Spectrum::compute)
		    .def("saveAndClean", &Spectrum::saveAndClean);
	}

private:

	shared_ptr<Options> _options;

};


class AtomicSpectrum
{
	AtomicSpectrum() {}
};

class SpectrumOverlap
{
	SpectrumOverlap() {}
};





/* STORAGE, BASIS, COMPUTATION, PARALLELIZATION */
/*
 *
 * Spectrum, PowerSpectrum
 * SpectrumK, PowerSpectrumK
 * StructureK, ParticleK (with weight, type)
 *
 * Basis
 * BasisFactory
 *
 * RadialBasis
 * RadialBasisLegendre
 * RadialBasisGaussian
 * RadialBasisHermite
 *
 * AngularBasis
 * AngularBasisHarmonic
 */


/*
 * Parallelization
 *     based on centers
 *     based on wavevectors
 */

/*
 * Spectrum->Setup(system, options)
 *     -> Basis->Setup()
 * Compute(patterns)
 *     for pattern in patterns:
 *        single out neighbours that match pattern
 *        for each neighbour:
 *            -> Compute(pos, refpos)
 */

/*
 * PowerSpectrum->Setup(Expansion)
 *      [a][b][n][n'][l] = sum{m} [a][nlm]*[b][n'lm]
 *
 * Spectrum in linear storage:
 * Start n at offset_n = (n-1)*(L+1)^2
 *     Start l at offset_l = offset_n + l^2
 *         Start m at offset_m = offset_l + (m+l)
 *
 * PowerSpectrum in linear storage:
 * Start n at offset_n = (n-1)*N*(L+1)
 *     Start n' at offset_n' = offset_n + (n'-1)*(L+1)
 *         Start l at offset_l = offset_n'
 *
 * map< pattern1, spectrum  >
 * for (nit in spectrum)
 *     for (lit in nit)
 *         for (mit in lit)
 *             nit->n
 *             lit->l
 *             mit->m
 *
 *
 */

/*
 * Storage:
 *     -> Serialize()
 *     For each center:
 *         for each pattern:
 *             [n][l][m]
 *     Store [n] and [l][m] separately?
 *
 *     -> PowerSerialize()
 *     For each center:
 *         for each pair of patterns:
 *             [n][n'][l]
 */

/*
 *
 * Types of basis functions: Legendre, Hermite, Gaussian*
 * Make sure to include appropriate n-factors, e.g. sqrt(2*n+1) for Legendre
 * Normalized on which interval?
 *
 * e.g. RadialBasisLegendre : RadialBasis
 *
 * RadialBasis->Setup()
 *     -> Orthogonalize()
 * RadialBasis -> EvaluateAt(radius)
 *     -> AtomicBasis->EvaluateAt(radius)
 *     -> Transform coefficients
 *     -> Return coefficients (N-vector)
 * RadialBasis->Iterator() ?
 *
 */

/*
 * AngularBasis->Setup()
 * AngularBasis->EvaluateAt(direction)
 *     -> AtomicBasisL->EvaluateAt(radius)
 *          -> AtomicBasisLM -> EvaluateAt(radius)
 *     -> Return coefficients (L*M-vector)
 * AngularBasis->Iterator()
 */



}}



#endif /* _SOAP_RADIALBASIS_HPP_ */
