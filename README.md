# nest-modmodule
This is a [NEST-simulator](http://www.nest-simulator.org/) module implementing a family of simple modulatory synapses.
It adds "modulatory_synapse", "d1_synapse" and "d2_synapse" to the nest dictionary of synapses.
All these models provide a mean to modulate the weights of a connection between neural populations based on the activity of a third population via a volume transmitter. They act by simply multiplying the baseline weight of the connection times the ratio fo spikes coming to the volume transmitter.
These synapses can be used to reproduce modulation as described in [Humphries et al. 2006](http://dx.doi.org/10.1523/JNEUROSCI.3486-06.2006)

As a general schema:
```
modulatory_synapse  :=   weight*ratio
d1_synapse  :=   weight*(1+alpha*ratio)
d2_synapse  :=   weight*(1-alpha*ratio)
d2div_synapse  :=   weight*/(1+alpha*ratio)

```

***Install:***
To compile and install the module just follow the [nest-developer tutorial for extension modules]( http://nest.github.io/nest-simulator/extension_modules)


***Dependencies:***
The module has been currently tested on the release of nest [2.10.0](http://www.nest-initiative.org/nestactivity/release-of-nest-2-10-0/)
