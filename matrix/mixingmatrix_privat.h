#ifndef MIXINGMATRIX_PRIVAT_H
#define MIXINGMATRIX_PRIVAT_H

#include <qvaluelist.h>

namespace JackMix {
namespace MixingMatrix {

class ElementFactory;

class Global
{
private:
	Global();
	~Global();
public:
	static Global* the();

	void registerFactory( ElementFactory* );
	void unregisterFactory( ElementFactory* );

	/**
	 * Returns the names of the Controls for the given number of in
	 * and out channels
	 */
	QStringList canCreate( int in, int out );

	/**
	 * Create a control of the given type and for the given in and
	 * out channels.
	 * Return true on success else false.
	 */
	bool create( QString type, QStringList ins, QStringList outs, Widget* parent, const char* =0 );

	void debug();
private:
	QValueList <ElementFactory*> _factories;
};

}; // MixingMatrix
}; // JackMix

#endif // MIXINGMATRIX_PRIVAT_H

