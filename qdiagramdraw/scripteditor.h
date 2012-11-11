#ifndef SCRIPTEDITOR_H
#define SCRIPTEDITOR_H

#include <QAbstractButton>
#include <QAction>
#include <QCloseEvent>
#include <QDialog>
#include <QFileInfo>
#include <QMainWindow>
#include <QScriptEngineDebugger>

class QFileSystemWatcher;
class AutoCompleteModel;

namespace Ui {
	class ScriptEditor;
}

class ScriptEditor : public QDialog
{
	Q_OBJECT
public:
	ScriptEditor(QWidget* parent = 0 );
	~ScriptEditor();
	AutoCompleteModel* autoCompleteModel() const;

	void setEngine(QScriptEngine* engine);
private slots:
	void abortScript();
	void appendMessage( const QString & text );
	void closePushButtonClicked();
	void openScript();
	void runScript();
	void runScriptInDebugger();
	void saveScript();
	void scriptTextEditTextChanged();
protected:
	void closeEvent( QCloseEvent* event );
private slots:
	void evaluationSuspended();
	void fileChanged( const QString & path );
private:
	void evaluate( bool debug = false);
	void initPrintFunction();
	bool loadFromFile( const QString & fileName );

	bool m_abort;
	QFileInfo m_fileInfo;
	QFileSystemWatcher* m_fileSystemWatcher;
	QString m_script;
	QScriptEngineDebugger* m_debugger;
	QMainWindow* m_debuggerWindow;
	QScriptEngine* m_engine;

	Ui::ScriptEditor* ui;
};

#endif // SCRIPTEDITOR_H
