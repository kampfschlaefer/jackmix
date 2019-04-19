/*
    Copyright 2005 - 2007 Arnold Krille <arnold@arnoldarts.de>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation;
    version 2 of the License.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.

*/

#ifndef BACKEND_INTERFACE_H
#define BACKEND_INTERFACE_H

#include <QtCore/QStringList>
#include <QtCore/QTime>
#include <QObject>

#include <QtCore/QDebug>

#include "guiserver_interface.h"

#define NUM_INTERPOLATION_STEPS 1024

class QDomElement;
class QDomDocument;

namespace JackMix {
	/**
	 * @brief Abstract interface for backends
	 *
	 * A backend has to implement this functions...
	 */
	class BackendInterface : public QObject
	{
Q_OBJECT
	public:
		BackendInterface( GuiServer_Interface* );
		virtual ~BackendInterface();

		/**
		 * @brief Return the current list of output channels.
		 */
		virtual const QStringList &outchannels() const =0;
		/**
		 * @brief Return the current list of input channels.
		 */
		virtual const QStringList &inchannels() const =0;

		/**
		 * @brief Set the volume of the named node.
		 */
		virtual void setVolume( QString, QString, float ) =0;
		/**
		 * @brief Set the volume of the named node.
		 */
		virtual float getVolume( QString, QString ) =0;
		/**
		 * @brief Add a channel and return true on success.
		 *
		 * If the actual backend doesn't support adding and removing, thats
		 * okay. Just return false..
		 */
		virtual bool addOutput( QString ) =0;
		/**
		 * @brief Add a channel and return true on success.
		 *
		 * If the actual backend doesn't support adding and removing, thats
		 * okay. Just return false..
		 */
		virtual bool addInput( QString ) =0;
		/**
		 * @brief Rename the output.
		 * 
		 * The output channel is renamed, checking first that the give name
		 * is not already associated with another output.
		 */
		virtual bool renameInput( const QString, const QString ) =0;
		/**
		 * @brief Rename the output.
		 * 
		 * The output channel is renamed, checking first that the give name
		 * is not already associated with another output.
		 */
		virtual bool renameOutput( const QString, const QString ) =0;
		/**
		 * @brief Remove a channel and return true on success.
		 *
		 * If the actual backend doesn't support adding and removing, thats
		 * okay. Just return false..
		 */
		virtual bool removeOutput( QString ) =0;
		/**
		 * @brief Remove a channel and return true on success.
		 *
		 * If the actual backend doesn't support adding and removing, thats
		 * okay. Just return false..
		 */
		virtual bool removeInput( QString ) =0;
                
                // The BackendInterface interface is assumed present in all backends.
                // This is responsible for making the flashy lights on the input
                // and output pots change colour. If you don't want to implement
                // that you can ignore it, and no signals will be sent; the input
                // and output potentiometers remain dark
                //
                // If you would like to impelment it, the idea is to record the
                // abs maximum level of each buffer (as floats in the range 0..1)
                // newInputLevel(QString key, float max) and newOutputLevel will
                // do that for you. When the levels for each input and output have
                // been recorded, call report() to signal the Widget to change the
                // colours of the input and output elements
		//
		// Similarly, of the backend of the dervied class uses its own
		// MIDI message handler, it can use the cc_message signal to notify
		// control changes.
                
	public:
		enum Level {none, nominal, high, too_high};
		Q_ENUM(Level)
		typedef QMap<QString,Level> levels_t;

		virtual void newInputLevel(QString which, float maxSignal);
		virtual void newOutputLevel(QString which, float maxSignal);
		Level signalToLevel(float sig) const;
		virtual void report();
		
	signals:
		void inputLevelsChanged(JackMix::BackendInterface::levels_t);
		void outputLevelsChanged(JackMix::BackendInterface::levels_t);
		void cc_message(int ch, int val);

	private:
		typedef struct {
			Level level;
			float max;
			bool  changed;
			QTime timeout;
		} Stats;

		QMap<QString, Stats> stats[2];

		void newLevel(Stats& s, float maxSignal); 
		void report_group(int which, levels_t& result);
		
		static const float threshold[];
		
	private slots:
		void testSlot(JackMix::BackendInterface::levels_t);
	protected:
		GuiServer_Interface* gui;
		/**
		 * @brief State of a fader (including interpolation)
		 * 
		 * Faders implemented in the backend need to interpolate the
		 * volume setting because otherwise sudden changes produce
		 * clicks in the output. Presently, the backend is expected
		 * (but not required) to change to a new volume setting from
		 * the previous one over the duration of one audio buffer.
		 */
		struct FaderState {

			float target;  /**< The target setting of the fader */
			float current; /**< The current setting of the fader,
				            changing troughout interpolation */
			unsigned int num_steps;
			unsigned int cur_step;
			FaderState(float initial=0, unsigned int _num_steps=NUM_INTERPOLATION_STEPS)
				: target {initial}, current{initial}
				, num_steps{_num_steps}, cur_step {0}
			{ }

			/**
			 * Assigning a target value. Interpolation will restart
			 * from the current value towards the new target.
			 */
			FaderState& operator=(float volume) {
				if (!qFuzzyCompare(current, target))
					// Time to change current in case we're not finished interpolating
					current += cur_step*(target - current)/num_steps;
				target = volume;
				cur_step = 0;
				
				return *this;
			}
		};
		
		/**
		 * @brief Perform interpolated volume scaling.
		 * 
		 * The volume interpolation is controlled by the parameter
		 * fs, a FaderState reference. The current field of fs is
		 * updated accordingly.
		 * 
		 * This assumes samples are stored in a buffer which can be
		 * subscripted via [] and that they are all multiplied
		 * by a constant which changes continuously througout
		 * the length of the buffer. If that isn't true for the
		 * backend implementing this interface, it will have
		 * to provide its own code. You can't have virtual
		 * templates.
		 * 
		 * @param buf     A pointer to a buffer of samples to process
		 * @param nframes Number of frames (= samples) in the buffer
		 * @param fs      A FaderState to control the fading
		 * @returns       Maximum sample value in the buffer
		 */
		template <typename SampT>
		inline SampT interp_fader(SampT* buf, const size_t nframes, FaderState& fs) {
			SampT max {0};

			if ( !qFuzzyCompare(fs.target, fs.current)) {
				for (size_t n {0}; n < nframes; n++) {
					buf[n] *= fs.current + fs.cur_step*(fs.target - fs.current)/fs.num_steps;
					max = qMax(max, buf[n]);
					fs.cur_step++;
					if (fs.cur_step == fs.num_steps) {
						fs.current = fs.target;
						fs.cur_step = 0;
					}
				}
			} else {
				for (size_t n {0}; n < nframes; n++) {
					buf[n] *= fs.current;
					max = qMax(max, buf[n]);
				}
			}
			return max;
		}
		
		/**
		 * @brief Add samples from one buffer to another with interplolated scaling
		 * 
		 * An overload of the in-place version of the template which leaves the
		 * input buffer unchanged but adds its data into the output buffer with
		 * interpolated scaling. Unlike the in-place version, the maximum value
		 * in the target buffer is not returned. This is because it is anticipated
		 * that the caller will mix a number of input buffers into the output
		 * by repeatedly calling this function, and since the maximum value is
		 * meaningless until that process is complete, it is simply a waste of
		 * computational effort. 
		 * 
		 * @see interp_fader(SampT* buf, const size_t nframes, FaderState& fs)
		 * 
		 * Similar assumptions about sample ordering and storage are made.
		 * 
		 * @param outbuf  Where to store result samples
		 * @param inbuf   Whence to read input samples
		 * @param nframes Number of frames (= samples) in the buffer
		 * @param fs      A FaderState to control the fading
		 */
		template <typename SampT>
		inline void interp_fader(SampT* outbuf, const SampT* inbuf,
					 const size_t nframes, FaderState& fs) {
			
			if ( !qFuzzyCompare(fs.target, fs.current)) {
				for (size_t n {0}; n < nframes; n++) {
					outbuf[n] += inbuf[n]*(fs.current + fs.cur_step*(fs.target - fs.current)/fs.num_steps);
					fs.cur_step++;
					if (fs.cur_step == fs.num_steps) {
						fs.current = fs.target;
						fs.cur_step = 0;
					}
				}
			} else {
				for (size_t n {0}; n < nframes; n++) {
					outbuf[n] += inbuf[n]*fs.target;
				}
			}
		}
	};
};

#endif // BACKEND_INTERFACE_H

