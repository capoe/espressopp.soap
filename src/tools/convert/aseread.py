import espressopp as sxx
import mpi4py.MPI as MPI

try:
    import ase
    import ase.io
except ImportError:
    print("Note: ase.io import failed. Install PYTHON-ASE to harvest full reader functionality.")

def setup_sxx_system(ase_config):
    
    # TODO Read from options
    r_cutoff = 5.
    r_cutoff_skin = 0.0

    # CONVERT "no-pbc" TO "pbc with vacuum" ...
    ase_cell = ase_config.cell
    box = [ase_cell[0], ase_cell[1], ase_cell[2]]
    for i in range(3):
        if not ase_config.pbc[i]:
            scale_i = 3.
            xis = ase_config.get_positions()[:,i]
            min_xi, max_xi = min(xis), max(xis)
            L_xi = scale_i*(max_xi-min_xi)
            box[i] = L_xi
            if box[i] < 2*(r_cutoff+r_cutoff_skin):
                box[i] = 2*(r_cutoff+r_cutoff_skin)

    # DEFINE SYSTEM
    system = sxx.System()
    system.rng = sxx.esutil.RNG()
    system.bc = sxx.bc.OrthorhombicBC(system.rng, box)
    system.skin = r_cutoff_skin
    node_grid = sxx.tools.decomp.nodeGrid(MPI.COMM_WORLD.size)
    cell_grid = sxx.tools.decomp.cellGrid(box, node_grid, r_cutoff, r_cutoff_skin)
    system.storage = sxx.storage.DomainDecomposition(system, node_grid, cell_grid)

    # PARTICLE PROPERTIES
    props = ['id', 'type', 'mass', 'pos']
    ids = [ i+1 for i in range(ase_config.get_number_of_atoms()) ]
    types = ase_config.get_atomic_numbers()
    positions = ase_config.get_positions()
    masses = ase_config.get_masses()

    # ADD PARTICLES & DECOMPOSE
    atom_tuples = []
    for i, typ, mass, pos in zip(ids, types, masses, positions):
        i_tuple = [ i, typ, mass, sxx.Real3D(pos[0], pos[1], pos[2]) ]
        atom_tuples.append(i_tuple)
    system.storage.addParticles(atom_tuples, *props)
    system.storage.decompose()

    return system
    

class AseConfigList(object):
    def __init__(self,
        config_files=[],
        log=None,
        grep_key='',
        read_fct=None,
        read_fct_args={}):
        if read_fct == None:
            import ase.io
            read_fct = ase.io.read
            read_fct_args = { 'index':':' }        
        self.configs = []
        config_idx = -1
        if log: log << "Reading configurations ..." << log.endl        
        for config_file in config_files:
            # Parse coordinate file
            try:
                if log: log << log.back << "..." << config_file << log.flush
                ase_configs = read_fct(config_file, **read_fct_args)
                if log: datastring = log >> log.catch >> 'cat %s | grep %s' % (config_file, grep_key)
                else: datastring = ''
            except KeyError:
                if log: log << endl << log.mr << "... Error when reading %s" % config_file << log.endl
                continue
            # Log & store configuration
            frame_idx = -1
            for ase_config in ase_configs:
                frame_idx += 1
                config_idx += 1
                config = AseConfig(ase_config=ase_config, 
                    config_idx=config_idx,
                    frame_idx=frame_idx,
                    config_file=config_file,
                    datastring=datastring)
                self.configs.append(config)
        if log: log << log.endl
    def __getitem__(self, idx):
        return self.configs[idx]
    def __iter__(self):
        return self.configs.__iter__()

class AseConfig(object):
    def __init__(self,
        ase_config=None,
        config_idx=None, 
        frame_idx=None, 
        config_file=None,
        datastring=None):
        # COORDINATES
        self.atoms = ase_config
        # BOOK-KEEPING
        self.config_idx = config_idx
        self.frame_idx = frame_idx
        self.config_file = config_file
        # DATA [E.G., FROM 2nd LINE IN XYZ]
        self.datastring = datastring
        self.data = datastring.split()
        # Z-STATISTICS
        self.z_count = {}
        for z in self.atoms.get_atomic_numbers():
            try:
                self.z_count[z] += 1
            except KeyError:
                self.z_count[z] = 1
        return
    def InfoString(self):
        z_string = 'N= %-2d ' % len(self.config.z)
        for z in Z_ELEMENT_LIST:
            z_string += 'Z[%d]= %-2d ' % (z, self.z_count[z])
        return "'%s'   Frame= %-4d Idx= %-6d %-40s" % (\
            self.config_file, self.frame_idx, 
            self.config_idx, z_string)
    def InfoStringVerbose(self):
        appendix = ''
        for d in self.data:
            appendix += '%-12s' % d
        return '%s RG= %+1.7e %s' % (self.InfoString(), self.RadiusOfGyration(), appendix)
    def GetData(self, idx, to_type=float):
        return to_type(self.data[idx])
    def RadiusOfGyration(self):
        radius_g2 = 0.
        com = self.atoms.get_center_of_mass()
        for pos in self.atoms.positions:
            dr = pos-com
            r2 = np.dot(dr,dr)
            radius_g2 += r2
        radius_g2 = radius_g2/len(self.atoms.positions)
        return radius_g2**0.5
        





