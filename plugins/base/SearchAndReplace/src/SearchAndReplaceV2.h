#ifndef SEARCHANDREPLACEV2_H
#define SEARCHANDREPLACEV2_H

#include <BasePlugin.h>

class SearchWidget;
class SearchResultsDock;

class SearchAndReplaceV2 : public BasePlugin
{
	Q_OBJECT
	Q_INTERFACES( BasePlugin )

public:
	enum ModeFlag
	{
		ModeFlagSearch = 0x1,
		ModeFlagReplace = 0x2,
		ModeFlagDirectory = 0x4,
		ModeFlagProjectFiles = 0x8,
		ModeFlagOpenedFiles = 0x10
	};
	
	enum Mode
	{
		ModeNo = 0,
		ModeSearch = ModeFlagSearch,
		ModeReplace = ModeFlagReplace,
		ModeSearchDirectory = ModeFlagSearch | ModeFlagDirectory,
		ModeReplaceDirectory = ModeFlagReplace | ModeFlagDirectory,
		ModeSearchProjectFiles = ModeFlagSearch | ModeFlagProjectFiles,
		ModeReplaceProjectFiles = ModeFlagReplace | ModeFlagProjectFiles,
		ModeSearchOpenedFiles = ModeFlagSearch | ModeFlagOpenedFiles,
		ModeReplaceOpenedFiles = ModeFlagReplace | ModeFlagOpenedFiles
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
	QPointer<SearchResultsDock> mDock;
	
	virtual void fillPluginInfos();

	virtual bool install();
	virtual bool uninstall();

protected slots:
	void searchFile_triggered();
	void replaceFile_triggered();
};

Q_DECLARE_OPERATORS_FOR_FLAGS( SearchAndReplaceV2::Options )

#endif // SEARCHANDREPLACEV2_H
