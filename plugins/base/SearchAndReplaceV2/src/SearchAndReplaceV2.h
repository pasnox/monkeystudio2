#ifndef SEARCHANDREPLACEV2_H
#define SEARCHANDREPLACEV2_H

#include <BasePlugin.h>

class SearchWidget;

class SearchAndReplaceV2 : public BasePlugin
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin )

public:
	enum Mode
	{
		ModeNo = 0,
		ModeSearch,
		ModeReplace,
		ModeSearchDirectory,
		ModeReplaceDirectory,
		ModeSearchProject,
		ModeReplaceProject,
		ModeSearchOpenedFiles,
		ModeReplaceOpenedFiles
	};
	
	enum Option
	{
		OptionNo = 0x0,
		OptionCaseSensitive = 0x1,
		OptionWholeWord = 0x2,
		OptionWrap = 0x4,
		OptionRegularExpression = 0x8,
	};
	
	Q_DECLARE_FLAGS( Options, Option );

protected:
	QPointer<SearchWidget> mWidget;
	
	virtual void fillPluginInfos();

	virtual bool install();
	virtual bool uninstall();
};

Q_DECLARE_OPERATORS_FOR_FLAGS( SearchAndReplaceV2::Options )

#endif // SEARCHANDREPLACEV2_H
