#include "stdafx.h"
#include "scripteditor.h"
#include "ui_scripteditor.h"

#include <scriptsyntaxhighlighter.h>

#include <QScriptEngine>

QScriptValue printFunction(QScriptContext *context, QScriptEngine *engine)
{
	QString result;
	for (int i = 0; i < context->argumentCount(); ++i) {
		if (i > 0)
			result.append(" ");
		result.append(context->argument(i).toString());
	}

	QScriptValue calleeData = context->callee().data();
	QPlainTextEdit* edit = qobject_cast<QPlainTextEdit*>(calleeData.toQObject());
	if (edit){
		edit->appendPlainText(result);
	}

	return engine->undefinedValue();
}

ScriptEditor::ScriptEditor(QWidget* parent) :
QDialog( parent ), ui(new Ui::ScriptEditor)
{
	ui->setupUi(this);

	ui->outputTextEdit->resize(ui->outputTextEdit->size().width(), ui->outputTextEdit->fontMetrics().height() * 3);
	m_abort = false;
	// Initialize file system watcher
	m_fileSystemWatcher = new QFileSystemWatcher(this);
	connect(m_fileSystemWatcher, SIGNAL(fileChanged(QString)), this, SLOT(fileChanged(QString)));
	// Initialize popup menu
	QMenu* m = 0;
	// Initialize execute push button pop menu
	m = new QMenu(this);
	m->addAction(ui->runScriptAction);
	connect(ui->runScriptAction, SIGNAL(triggered()), this, SLOT(runScript()));
	m->addAction(ui->runScriptInDebuggerAction);
	connect(ui->runScriptInDebuggerAction, SIGNAL(triggered()), this, SLOT(runScriptInDebugger()));
	m->addSeparator();
	m->addAction(ui->abortScriptAction);
	connect(ui->abortScriptAction, SIGNAL(triggered()), this, SLOT(abortScript()));
	ui->executePushButton->setMenu(m);
	//
	m = new QMenu(this);
	m->addAction(ui->openScriptAction);
	connect(ui->openScriptAction, SIGNAL(triggered()), this, SLOT(openScript()));
	m->addAction(ui->saveScriptAction);
	connect(ui->saveScriptAction, SIGNAL(triggered()), this, SLOT(saveScript()));
	ui->scriptPushButton->setMenu(m);
	
	m_debugger = new QScriptEngineDebugger(0);
	connect(m_debugger, SIGNAL(evaluationSuspended()), this, SLOT(evaluationSuspended()));

	// Initialize script engine
	m_engine = new QScriptEngine(this);
	m_engine->setProcessEventsInterval(100);
	initPrintFunction();

	QScriptValue diagramView = m_engine->newQObject(ui->diagramView);
	m_engine->globalObject().setProperty("diagramView", diagramView);

	//m_debugger->attachTo(m_engine);
	ui->stackedWidget->addWidget(m_debugger->standardWindow());

	ui->abortScriptAction->setEnabled(false);
	ui->runScriptAction->setEnabled(false);
	ui->runScriptInDebuggerAction->setEnabled(false);

	ui->splitter_2->setStretchFactor(0, 90);
}

ScriptEditor::~ScriptEditor()
{
	delete ui;
	delete m_debugger;
}

AutoCompleteModel* ScriptEditor::autoCompleteModel() const
{
	return ui->scriptTextEdit->autoCompleteModel();
}

void ScriptEditor::initPrintFunction()
{
	QScriptValue print = m_engine->newFunction(printFunction);
	print.setData(m_engine->newQObject(ui->outputTextEdit));
    m_engine->globalObject().setProperty("print", print);
}

void ScriptEditor::evaluationSuspended()
{
	if (m_abort){
		if (m_engine->isEvaluating()){
			m_debugger->action(QScriptEngineDebugger::RunToNewScriptAction)->trigger();
		}
	} else {
		ui->stackedWidget->setCurrentIndex(1);
	}
}

void ScriptEditor::abortScript()
{
	m_abort = true;
	if (m_debugger){
		m_debugger->action(QScriptEngineDebugger::RunToNewScriptAction)->trigger();
	}
	m_engine->abortEvaluation();
	// Disable abort action
	ui->abortScriptAction->setEnabled(false);
	qApp->processEvents();
	appendMessage(tr("Evaluation aborted."));
	// Enable script push button
	ui->scriptPushButton->setEnabled(true);
}

void ScriptEditor::appendMessage( const QString & text )
{
//	ui->outputTextEdit->appendPlainText(text);
}

void ScriptEditor::closeEvent( QCloseEvent* event )
{
	if (m_engine->isEvaluating()){
		event->ignore();
	} else if (ui->scriptTextEdit->toPlainText() != m_script){
		QString mText;
		if (m_fileInfo.absoluteFilePath().isEmpty()){
			mText = tr("The script contains unsaved changes. Do you want to save it?");
		} else {
			mText = QString(tr("The script '%1' contains unsaved changes. Do you want to save it?")).arg(m_fileInfo.fileName());
		}
		if (QMessageBox::question(this, tr("Unsaved changes"), mText, QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes){
			saveScript();
		}
		event->accept();
	} else {
		event->accept();
	}
}

void ScriptEditor::closePushButtonClicked()
{
	if (ui->scriptTextEdit->toPlainText() != m_script){
		QString mText;
		if (m_fileInfo.absoluteFilePath().isEmpty()){
			mText = tr("The script contains unsaved changes. Do you want to save it?");
		} else {
			mText = QString(tr("The script '%1' contains unsaved changes. Do you want to save it?")).arg(m_fileInfo.fileName());
		}
		if (QMessageBox::question(this, tr("Unsaved changes"), mText, QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes){
			saveScript();
		}
	}
	reject();
}

bool ScriptEditor::loadFromFile( const QString & fileName )
{
	//
	QFile f(fileName);
	if (f.open(QIODevice::ReadOnly)){
		//
		ui->scriptTextEdit->clear();
		// Read script
		QTextStream mStream(&f);
		m_script = mStream.readAll();
		//
		ui->scriptTextEdit->setPlainText(m_script);
		m_script = ui->scriptTextEdit->toPlainText();
		// Update labels
		ui->fileNameLabel->setText(m_fileInfo.fileName());
		ui->scriptChangedLabel->setPixmap(QPixmap(":/qdiagramdraw/script.unchanged"));
		// Disable save script action
		ui->saveScriptAction->setEnabled(false);
		f.close();
		// Clear previious file system watcher settings
		Q_FOREACH(QString f,m_fileSystemWatcher->files()){
			m_fileSystemWatcher->removePath(f);
		}
		// Update file system watcher
		m_fileSystemWatcher->addPath(m_fileInfo.absoluteFilePath());
		return true;
	} else {
		QMessageBox::critical(this, tr("File Error"), f.errorString());
		return false;
	}
}

void ScriptEditor::openScript()
{
	QSettings s;
	// Check for unsaved changes
	if (ui->scriptTextEdit->toPlainText() != m_script){
		if (QMessageBox::question(this, tr("Unsaved Changes"), tr("The script contains unsaved changes. Do you want to discard the changes?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::No){
			return;
		}
	}
	// Get open filename
	QString name = QFileDialog::getOpenFileName(this, tr("Load Script"), s.value("recentScriptFolder", qApp->applicationDirPath()).toString(), tr("Scripts (*.js)"));
	if (!name.isEmpty()){
		m_fileInfo.setFile(name);
		//
		s.setValue("recentScriptFolder", m_fileInfo.absolutePath());
		//
		loadFromFile(name);
	}
}

void ScriptEditor::runScript()
{
	evaluate();
}

void ScriptEditor::runScriptInDebugger()
{
	evaluate(true);
}

void ScriptEditor::saveScript()
{
	QSettings s;
	QString name;

	name = m_fileInfo.absoluteFilePath();
	if (name.isEmpty()){
		name = s.value("recentScriptFolder", qApp->applicationDirPath()).toString() + "/new.js";
	}
	name = QFileDialog::getSaveFileName(this, tr("Save Script"),name , tr("Scripts (*.js)"));
	if (!name.isEmpty()){
		// Inhibit notifications from file system watcher
		m_fileSystemWatcher->blockSignals(true);
		QFile f(name);
		if (f.open(QIODevice::ReadWrite | QIODevice::Truncate)){
			QTextStream stream(&f);
			// Write plain text
			stream << ui->scriptTextEdit->toPlainText();
			// Close file
			f.close();
			// Update file info
			m_fileInfo.setFile(name);
			// Update labels and icons
			ui->fileNameLabel->setText(m_fileInfo.fileName());
			ui->scriptChangedLabel->setPixmap(QPixmap(":/qdiagramdraw/script.unchanged"));
			//
			m_script = ui->scriptTextEdit->toPlainText();
			// Disable save script action
			ui->saveScriptAction->setEnabled(false);
			// Inform user
			QMessageBox::information(this, tr("File saved"), QString(tr("Script saved as %1")).arg(m_fileInfo.absoluteFilePath()));
		} else {
			QMessageBox::critical(this, tr("File Error"), f.errorString());
		}
		m_fileSystemWatcher->blockSignals(false);
	}
}

void ScriptEditor::evaluate( bool debug )
{
	ui->outputTextEdit->clear();
	// 
	ui->scriptTextEdit->clearMarkedLines();
	// Garbage collection
	m_engine->collectGarbage();
	// Disable buttons
	ui->runScriptAction->setEnabled(false);
	ui->runScriptInDebuggerAction->setEnabled(false);
	ui->abortScriptAction->setEnabled(true);
	// Process pending events
	qApp->processEvents();
	
	//
	if (debug){
		// Initialize script debugger
		//m_debugger = new QScriptEngineDebugger(this);
		//m_debuggerWindow = m_debugger->standardWindow();
		//m_debuggerWindow->setWindowModality(Qt::ApplicationModal);
		//ui->stackedWidget->addWidget(m_debugger->standardWindow());
		//ui->stackedWidget->setCurrentIndex(1);
		// Attach debbugger to the engine
		m_debugger->attachTo(m_engine);
		ui->stackedWidget->setCurrentIndex(1);
		// Trigger interrupt action to break at the first line
		m_debugger->action(QScriptEngineDebugger::InterruptAction)->trigger();
	} else {
		// Detach debugger from engine
		 m_debugger->detach();
	}
	// Evaluate script
	QScriptValue v = m_engine->evaluate(ui->scriptTextEdit->toPlainText(), ui->fileNameLabel->text());
	m_abort = false;
	// Hide debugger window
	if (debug){
		ui->stackedWidget->setCurrentIndex(0);
	}
	//
	if (m_engine->hasUncaughtException()){
		ui->outputTextEdit->appendPlainText(tr("Exception: %1 in line: %2").arg(m_engine->uncaughtException().toString()).arg(m_engine->uncaughtExceptionLineNumber()));
		ui->scriptTextEdit->markLineAsError(m_engine->uncaughtExceptionLineNumber());
	} else {
		ui->stackedWidget->setCurrentIndex(0);
		ui->outputTextEdit->appendPlainText(v.toString());
	}
	// Enable buttons
	ui->runScriptAction->setEnabled(true);
	ui->runScriptInDebuggerAction->setEnabled(true);
	ui->abortScriptAction->setEnabled(false);
	// Process pending events
	qApp->processEvents();
}

void ScriptEditor::fileChanged( const QString & path )
{
	// Check if the current (loaded) file has changed
	if (path == m_fileInfo.absoluteFilePath()){
		// Prevent calling this method multiple times
		m_fileSystemWatcher->blockSignals(true);
		// Check if the current file still exists
		if (m_fileInfo.exists()){
			QString mText;
			mText = QString(tr("%1\n\nThis file has been modified by another program!\nDo you want to reload it?"))
				.arg(m_fileInfo.absoluteFilePath());
			if (QMessageBox::question(this, tr("Reload"), mText, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes){
				loadFromFile(m_fileInfo.absoluteFilePath());
			}
		} else {
			// The file has been delete/moved.
			QString mText;
			mText = QString(tr("The file \"%1\" doesn't exist anymore. Keep this file in editor?"))
				.arg(m_fileInfo.absoluteFilePath());
			if (QMessageBox::question(this, tr("Keep non existing file"), mText, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::No){
				QSettings mSettings;
				// Update file name
				m_fileInfo = QFileInfo(mSettings.value("recentScriptFolder", qApp->applicationDirPath()).toString() + "/new.js");
				// Clear script editor
				ui->scriptTextEdit->clear();
				// 
				m_script = ui->scriptTextEdit->toPlainText();
				ui->fileNameLabel->setText(m_fileInfo.fileName());
				ui->scriptChangedLabel->setPixmap(QPixmap(":/qdiagramdraw/script.unchanged"));
			}
		}
		m_fileSystemWatcher->blockSignals(false);
	}
}

void ScriptEditor::scriptTextEditTextChanged()
{
	ui->runScriptAction->setEnabled(!ui->scriptTextEdit->toPlainText().isEmpty());
	ui->runScriptInDebuggerAction->setEnabled(ui->runScriptAction->isEnabled());
	// Check if the code has changed
	if (ui->scriptTextEdit->toPlainText() != m_script){
		ui->scriptChangedLabel->setPixmap(QPixmap(":/qdiagramdraw/script.changed"));
		ui->saveScriptAction->setEnabled(true);
	} else {
		ui->scriptChangedLabel->setPixmap(QPixmap(":/qdiagramdraw/script.unchanged"));
		ui->saveScriptAction->setEnabled(false);
	}
}

void ScriptEditor::setEngine(QScriptEngine* engine)
{
	if (m_engine->parent() == this){
		m_debugger->detach();
		delete m_engine;
	} else {
		m_debugger->detach();
	}
	m_engine = engine;
	initPrintFunction();
	QScriptValue diagramView = m_engine->newQObject(ui->diagramView);
	m_engine->globalObject().setProperty("diagramView", diagramView);
}
