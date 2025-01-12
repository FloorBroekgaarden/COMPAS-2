###################################################################
#                                                                 #                                                               
#  Example of plotting detailed output COMPAS with python         #
#                                                                 #
# ##################################################################

import pandas as pd
import numpy as np
import h5py as h5
import matplotlib.pyplot as plt
from matplotlib.legend import Legend
import matplotlib.ticker as ticker  
import os, sys

def main():

    ### Read file and create dataframe.
    data_path = './COMPAS_Output/Detailed_Output/BSE_Detailed_Output_0.h5'
    Data = h5.File(data_path, 'r')

    ### Creating new column for total mass
    Mtot = Data['Mass(1)'][()] + Data['Mass(2)'][()]
    
    ### Create subplots arranged in a 2x3 table, sharing x-axes whenever plots have the same x-axis range.
    fig, axes = plt.subplots(2,2, sharex=True, figsize=(10, 6)) # Dimensions chosen for aesthetics in the paper
    
    ### Plot mass attributes 
    lines = []  # This is used to split the legend into 2 pieces
    lines += axes[0][0].plot(Data['Time'][()], Mtot, linestyle='-', c='k', label='System Mass')
    lines += axes[0][0].plot(Data['Time'][()], Data['Mass(1)'][()], linestyle='-', c='r', label='Total Mass 1')
    lines += axes[0][0].plot(Data['Time'][()], Data['Mass_He_Core(1)'][()], linestyle='--', c='r', label='He Core 1')
    lines += axes[0][0].plot(Data['Time'][()], Data['Mass_CO_Core(1)'][()], linestyle=':', c='r', label='CO Core 1')
    lines += axes[0][0].plot(Data['Time'][()], Data['Mass(2)'][()], linestyle='-', c='b', label='Total Mass 2')
    lines += axes[0][0].plot(Data['Time'][()], Data['Mass_He_Core(2)'][()], linestyle='--', c='b', label='He Core 2')
    lines += axes[0][0].plot(Data['Time'][()], Data['Mass_CO_Core(2)'][()], linestyle=':', c='b', label='CO Core 2')
    axes[0][0].set_ylabel(r'Mass $/ \; M_{\odot}$')
    axes[0][0].grid(linestyle=':', c='gray')
    axes[0][0].legend(lines[:4], axes[0][0].get_legend_handles_labels()[1][:4], prop={'size':8}, loc=(0.05, 0.55))
    axes[0][0].add_artist(Legend(axes[0][0], lines[4:], axes[0][0].get_legend_handles_labels()[1][4:], prop={'size':8}, loc=(0.05, 0.15)))
    axes[0][0].tick_params(labelbottom=True)
    axes[0][0].xaxis.set_major_locator(ticker.MultipleLocator(2.0))
    axes[0][0].xaxis.set_minor_locator(ticker.MultipleLocator(1.0))
    axes[0][0].set_xlim(-0.5, 9)
    
    ### Plot radius attributes 
    axes[0][1].plot(Data['Time'][()], Data['SemiMajorAxis'][()], linestyle='-', c='k', label='Semi-Major Axis')
    axes[0][1].plot(Data['Time'][()], Data['Radius(1)'][()]/Data['Radius(1)|RL'][()], linestyle='--', c='r', label='Roche Radius 1')
    axes[0][1].plot(Data['Time'][()], Data['Radius(2)'][()]/Data['Radius(2)|RL'][()], linestyle='--', c='b', label='Roche Radius 2')
    axes[0][1].plot(Data['Time'][()], Data['Radius(1)'][()], linestyle='-', c='r', label='Stellar Radius 1')
    axes[0][1].plot(Data['Time'][()], Data['Radius(2)'][()], linestyle='-', c='b', label='Stellar Radius 2')
    axes[0][1].set_ylabel(r'Radius $/ \; R_{\odot}$')
    axes[0][1].set_yscale('log')
    axes[0][1].grid(linestyle=':', c='gray')
    axes[0][1].legend(prop={'size':8})
    axes[0][1].tick_params(labelbottom=True)
    
    ### Plot eccentricity
    axes[1][0].plot(Data['Time'][()], Data['Eccentricity'][()], linestyle='-', c='k') #, label= 'Eccentricity')
    axes[1][0].set_ylabel('Eccentricity')
    axes[1][0].set_xlabel('Time / Myr')
    axes[1][0].set_ylim(-0.05, 1.05)
    axes[1][0].grid(linestyle=':', c='gray')
    
    ### Plot stellar types
    stellarTypes, useTypes, typeNameMap = getStellarTypes(Data)
    
    axes[1][1].plot(Data['Time'][()], typeNameMap(Data['Stellar_Type(1)'][()]), linestyle='-', c='r', label='Stellar Type 1')
    axes[1][1].plot(Data['Time'][()], typeNameMap(Data['Stellar_Type(2)'][()]), linestyle='-', c='b', label='Stellar Type 2')
    axes[1][1].set_ylabel('Stellar Type')
    axes[1][1].set_xlabel('Time / Myr')
    axes[1][1].grid(linestyle=':', c='gray')
    axes[1][1].legend(prop={'size':8}) #, loc='lower left')
    axes[1][1].set_yticks(range(useTypes.shape[0]))
    axes[1][1].set_yticklabels([stellarTypes[typeNum] for typeNum in useTypes])
    axes[1][1].tick_params(width=2, labelsize=10)
    axes[1][1].yaxis.grid(True)
    
    ### Add in plot letters # The specific values were chosen for aesthetics in the COMPAS methods paper
    axes[0][0].text( axes[0][0].get_xlim()[1]*-.18, axes[0][0].get_ylim()[1]*0.87,  'a)', fontweight='bold')
    axes[0][1].text( axes[0][1].get_xlim()[1]*-.25, axes[0][1].get_ylim()[1]**0.75, 'b)', fontweight='bold') # exponent because it's on a log axis
    axes[1][0].text( axes[1][0].get_xlim()[1]*-.18, axes[1][0].get_ylim()[1]*0.87,  'c)', fontweight='bold')
    axes[1][1].text( axes[1][1].get_xlim()[1]*-.25, axes[1][1].get_ylim()[1]*0.87,  'd)', fontweight='bold')
    
    ### Finalize the boundaries, save, and show
    fig.subplots_adjust(left=0.05)  #adjusting boundaries of the plotter
    fig.subplots_adjust(wspace=.3)
    plt.savefig('gw151226evol.eps', format='eps') 
    plt.savefig('gw151226evol.png', format='png') 
    plt.show()




### Helper functions

def getStellarTypes(Data):
    """
    This function extracts only the stellar types which actually arise in the binary's evolution,
    and produces a map between the used type numbers and names.
    """

    # List of Hurley stellar types
    stellarTypes = [r'MS$<0.7M_{\odot}$', r'MS$\geq0.7M_{\odot}$', 'HG', 'FGB', 'CHeB', 'EAGB', 'TPAGB', 'HeMS', 'HeHG', 'HeGB', 'HeWD', 'COWD', 'ONeWD', 'NS', 'BH', 'MR']

    useTypes = np.unique(np.append(Data['Stellar_Type(1)'][()], Data['Stellar_Type(2)'][()]))
    if (0 in useTypes) != (1 in useTypes): # XOR
        stellarTypes[0] = stellarTypes[1] = 'MS'

    def typeNameMap(x):
        return np.digitize(x, useTypes, right=True) 

    return stellarTypes, useTypes, typeNameMap

""
if __name__ == "__main__":
    main()


""

