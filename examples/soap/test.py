import espressopp as sxx
from momo import osio, endl, flush
import mpi4py.MPI as MPI
import os

print MPI.COMM_WORLD.size

# COLLECT CONFIG FILES FROM FOLDER
config_file_dir = './configs'
osio.cd(config_file_dir)
config_files = sorted(os.listdir('./'))
config_list = sxx.tools.convert.aseread.AseConfigList(config_files, log=osio)
osio.cd(-1)

# SETUP SYSTEMS FOR CONFIGURATIONS
systems = []
for config in config_list:
    osio << config.atoms << endl
    print config.atoms.get_initial_charges()
    system = sxx.tools.convert.aseread.setup_sxx_system(config.atoms)
    systems.append(system)


options = sxx.soap.Options()
options.configureCenters(1., [1,77,119])
options.configureRealBasis(12,9,10.)
options.configureReciprocalBasis(10,7,8.)

b1 = sxx.Real3D(2.,0,0)
b2 = sxx.Real3D(0,2.,0)
b3 = sxx.Real3D(0,0,2.)
options.configureReciprocalLattice(b1, b2, b3)

print options.summarizeOptions()


for system in systems:
    tmp = sxx.soap.Soap(system)
    print "soap::compute"
    tmp.compute()
    portal = sxx.soap.Portal(system)
    print "portal::initialise"
    portal.initialise()

osio.okquit()











L           = 16
box         = (L, L, L)
rc          = 2 * pow(2, 1./6.)
skin        = 0.3
dt          = 0.001
epsilon     = 1.
sigma       = 0.
temperature = 1.2

bonds, angles, x, y, z, Lx, Ly, Lz = espressopp.tools.convert.lammps.read('polymer_melt.lammps')
bonds, angles, x, y, z, Lx, Ly, Lz = espressopp.tools.replicate(bonds, angles, x, y, z, Lx, Ly, Lz, xdim=1, ydim=1, zdim=1)
num_particles = len(x)
density = num_particles / (Lx * Ly * Lz)
box = (Lx, Ly, Lz)
system, integrator = espressopp.standard_system.Default(
    box=box, 
    rc=rc, 
    skin=skin, 
    dt=timestep, 
    temperature=temperature)




# add particles to the system and then decompose
# do this in chunks of 1000 particles to speed it up
props = ['id', 'type', 'mass', 'pos']
new_particles = []
for i in range(num_particles):
  part = [i + 1, 0, 1.0, espressopp.Real3D(x[i], y[i], z[i])]
  new_particles.append(part)
  if i % 1000 == 0:
    system.storage.addParticles(new_particles, *props)
    system.storage.decompose()
    new_particles = []
system.storage.addParticles(new_particles, *props)
system.storage.decompose()

# SYSTEM
system         = sxx.System()
system.rng     = sxx.esutil.RNG()
system.bc      = sxx.bc.OrthorhombicBC(system.rng, box)
system.skin    = skin

# GRID
nodeGrid       = sxx.tools.decomp.nodeGrid(sxx.MPI.COMM_WORLD.size)
cellGrid       = sxx.tools.decomp.cellGrid(box, nodeGrid, rc, skin)
system.storage = sxx.storage.DomainDecomposition(system, nodeGrid, cellGrid)

# INTERACTIONS
interaction    = sxx.interaction.VerletListLennardJones(sxx.VerletList(system, cutoff=rc))
potLJ          = sxx.interaction.LennardJones(epsilon, sigma, rc)
interaction.setPotential(type1=0, type2=0, potential=potLJ)
system.addInteraction(interaction)

# INTEGRATOR
integrator     = sxx.integrator.VelocityVerlet(system)
integrator.dt  = dt
thermostat     = sxx.integrator.LangevinThermostat(system)
thermostat.gamma  = 1.0
thermostat.temperature = temperature
integrator.addExtension(thermostat)


props    = ['id', 'type', 'mass', 'pos', 'v']
vel_zero = sxx.Real3D(0.0, 0.0, 0.0)

bondlist = sxx.FixedPairList(system.storage)
pid      = 1
p_type   = 0
mass     = 1.0
chain    = []
n_chains = 1
monomers_per_chain = 10
bondlen = 1.

system.storage.decompose()
potFENE   = sxx.interaction.FENE(K=30.0, r0=0.0, rMax=1.5)
interFENE = sxx.interaction.FixedPairListFENE(system, bondlist, potFENE)
system.addInteraction(interFENE)

print "dt    =", integrator.dt
print "gamma =", thermostat.gamma
print "T     =", thermostat.temperature

tmp = sxx.analysis.Temperature(system)
print "temp::compute", tmp.compute()

tmp = sxx.soap.Soap(system)
print "soap::compute", tmp.compute()

