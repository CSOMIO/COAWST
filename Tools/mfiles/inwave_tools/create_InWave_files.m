% create_InWave_files.m
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% This script generates all the nc files needed to run the InWave model: 
% InWave_grd.nc
% InWave_ini.nc
% InWave_bnd.nc
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


%%%%%%%%%%%%%%%%%%%%%%%%%%% USER SECTION %%%%%%%%%%%%%%%%%%%%%%%%%%
%
%     The user has to define which application is going to run
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%1) SET THE CASE TO = 1.

STEP=1;
SOLITON=0;
BEACH=0;
SHOAL=0;
JET=0;
DIR_TRANSPORT=0;
MY_APP=0;


%2) Provide the name of the mfile containing configuration parameters of
% the run; 

if (STEP) 
  inwave_gen_file='InWave_step_param';
elseif (SOLITON)
  inwave_gen_file='InWave_soliton_param';
elseif (BEACH)
  inwave_gen_file='InWave_beach_param';
elseif (SHOAL)
  inwave_gen_file='InWave_shoal_param';
elseif (JET)
  inwave_gen_file='InWave_jet_param';
elseif (DIR_TRANSPORT)
  inwave_gen_file='InWave_dir_transport_param';
elseif (MY_APP)
  inwave_gen_file='InWave_myapp_param';
end

%%%%%%%%%%%%%%%%%%%%% END OF USER SECTION %%%%%%%%%%%%%%%%%%%%%%%%%

%2.5 Includes the configuration parameters

eval([inwave_gen_file])

%3) CREATE InWave GRID FILE

create_inwave_grid(x,y,dx,dy,depth,roms_angle,mask_rho,f,grd_file)

%4) CREATE InWave INI FILE

create_inwave_ini(Lm,Mm,Nbins,Bindirs,pd,Ac,Cx,Cy,Ct,TA,ini_file)

%5) CREATE InWave BND FILE

if (bin_error==1)
  
  disp([' ERROR WHEN CREATING INWAVE BOUNDARY FILE:'])
  disp([' You need to change the direction of the bins containing the energy'])
  disp([' At least one of them does not coincide with the considered central bin angles'])
  disp([' Select one of the following:'])
  disp([Bindirs])

else

  if obc(1)==1
  create_inwave_bnd(Lm, Mm, Nbins_bnd, dir_bnd, obc, ...
    Ac_north,TA,time, bnd_file)
  elseif obc(2)==1
  create_inwave_bnd(Lm, Mm, Nbins_bnd, dir_bnd, obc, ...
    Ac_east,TA,time, bnd_file)
  elseif obc(3)==1
  create_inwave_bnd(Lm, Mm, Nbins_bnd, dir_bnd, obc, ...
    Ac_south,TA,time, bnd_file)
  elseif obc(4)==1
  create_inwave_bnd(Lm, Mm, Nbins_bnd, dir_bnd, obc, ...
    Ac_west,TA,time, bnd_file)
  end

  %6) END OF FILE GENERATION

  disp(['INWAVE FILES CREATED'])

end

