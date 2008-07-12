
#ifndef GDBWATCHSTRUCT_H
#define GDBWATCHSTRUCT_H

#include <QString>
#include <QStringList>
#include <QTreeWidgetItem>

//! This class implements all functions for unformated structure from Gdb. Used by GdbWatch class 
/**
\author xiantia
\version 1.3.2
*/
class WatchStruct
{

private:

	// extrait les membres de la  structure
	// exemple : "rtf = {art = {0}, a = 5 , b = 6 , rtf  = {t = 12 , r = 34}}"
	// return  :
	// 1 : art = {0}
	// 2 : a = 5
	// 3 : b = 6
	// 4 : rtf = {t = 12 , r = 34}

	/**
	 * \brief Extract all members in struture
	*/
	QStringList extractMember(QString val);


	// return le type du membre de la structure
	// 2 : c'est un autre structure
	// 1 : membre " x= y"
	// 0 : membre orphelin "0x00"

	/**
	 * \brief Return the type of member present in struture.
	*/
	int memberType(QString val);


	// formatage des membres
	/**
	 * \brief Return the member correctly formated.
	*/
	QStringList formatMember(QString val);


	// recursive function
public:

	/**
	 * \brief Unformated structure from Gdb and create child of QTreeWidget.
	 *
	 * For example Gdb send myStruct = { a = 3, b = { x = 4, y = 6}}.
	 * After formated you see correctly this struture in QTreeWidget
	 *
	 * \code 
	 * Var Name			Value
	 * myStruct
	 *	|_ a			3
	 *	|_ b			
	 *		|_ x		4
	 *		|_ y		6
	 * \endcode
	*/
	void decompilStrut(QTreeWidgetItem *parentItem, QString val);
};

#endif
