# ofxPro for openFrameworks

Prototyping tool for OpenFrameworks based on the core environment of [CLOUDS](http://www.cloudsdocumentary.com/)'s VisualSystems that we work together with [James George](http://www.jamesgeorge.org/) and [Reza Ali](http://www.syedrezaali.com/). **ofxPro** is a highly modular implementation of Reza's **Rezonator**, that:

- save and load presets on the fly.

- make your own UI modules and integrate them easily to the prototyping environment to animate it. 

- construct and edit 3D Scene by dragging lights, changing cameras, and applying FOG and other effects.

- code shaders on realtime using your default text editor and animate the passed uniforms.

- document your work by recording or taking snapshots of it. This documentation can be upload automatically to Flickr.

- hand write notes on the 2D and 3D scenes to share with the rest of the team.


## INSTALL
		
Require by core:

	git clone --depth 1 https://github.com/rezaali/ofxUI.git

**NOTE:** currently there is a bug with the last version of ```ofxUI```. A work around is to detach the head of it to ```a04c286```

	cd openFrameworks/addons/ofxUI
	git checkout a04c286

## USE



## Hot Keys

* h : show/hide the UI
* Ctrl + f : toggle fullscreen mode
* Ctrl + s : save (if LOG is enable uploads a screenshot to flickr)
* Ctrl + S : save as... (make a preset)
* Ctrl + r : start/stop recording (if LOG is enable uploads the video to flickr)
* Ctrl + d : enable/disable DebugMode
* Ctrl + e : enable/disable EditMode (let you drag lights)