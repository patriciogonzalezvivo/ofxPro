# ofxPro for openFrameworks

Prototyping tool for OpenFrameworks based on the core environment of CLOUDS's VisualSystems that we work together with [James George](http://www.jamesgeorge.org/) and [Reza Ali](http://www.syedrezaali.com/). **ofxPro** Is an integrations of Reza's **Rezonator** with James **ofxTimeline**, put together in  a hight modular structure, that let you:

- record, load and animate presets on the fly.

- make your own UI modules and integrate them easily to the prototyping environment to animate it. 

- construct and edit 3D Scene by dragging lights, changing cameras, and applying FOG and other effects.

- code shaders on realtime using your default text editor and animate the passed uniforms.

- document your work by recording or taking snapshots of it. This documentation can be upload automatically to Flickr.

- hand write notes on the 2D and 3D scenes to share with the rest of the team.


## INSTALL
		
Require by core:

	git clone --depth 1 https://github.com/rezaali/ofxUI.git

## USE




## Hot Keys

* h : show/hide the UI
* Ctrl + f : toggle fullscreen mode
* Ctrl + s : save (if LOG is enable uploads a screenshot to flickr)
* Ctrl + S : save as... (make a preset)
* Ctrl + r : start/stop recording (if LOG is enable uploads the video to flickr)
* Ctrl + d : enable/disable DebugMode
* Ctrl + e : enable/disable EditMode (let you drag lights)

## TIMELINE-PRO extension

### Install

Require by ofxTimeline: 

	git clone --depth 1 https://github.com/Flightphase/ofxTextInputField.git

	git clone --depth 1 https://github.com/YCAMInterlab/ofxTimecode.git
  
	git clone --depth 1 https://github.com/ofxMSATimer/ofxMSATimer.git
  
	git clone --depth 1 https://github.com/Flightphase/ofxRange.git
  
	git clone --depth 1 https://github.com/arturoc/ofxTween.git
  
	git clone --depth 1 https://github.com/YCAMInterlab/ofxTimeline.git


### How to Animate with timelinePro?

1. Enable the TIMELINE TAB
2. Click on "Animate"
3. Click any UI element
4. A timeline will apear. Edit it.
5. Check that TIMELINE TAB is enable
6. Position the starting time on the timeline and hit SPACEBAR