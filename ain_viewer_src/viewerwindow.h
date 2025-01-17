// Copyright (c) 2021 Rumen G.Bogdanovski
// All rights reserved.
//
// You can use this software under the terms of 'INDIGO Astronomy
// open-source license' (see LICENSE.md).
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHORS 'AS IS' AND ANY EXPRESS
// OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
// GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


#ifndef VIEWERWINDOW_H
#define VIEWERWINDOW_H

#include <stdio.h>
#include <QApplication>
#include <QMainWindow>
#include <QComboBox>
#include <indigo/indigo_bus.h>
#include <imageviewer.h>
#include <imagepreview.h>
#include <textdialog.h>

#include <conf.h>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMenuBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QProgressDialog>
#include <QThread>
#include <QtConcurrentRun>
#include <QProgressBar>


class ViewerWindow : public QMainWindow {
	Q_OBJECT
public:
	explicit ViewerWindow(QWidget *parent = nullptr);
	virtual ~ViewerWindow();
	void open_image(QString file_name);
	void show_message(const char *title, const char *message, QMessageBox::Icon icon = QMessageBox::Warning);
	void block_scrolling(bool blocked) {
		if (blocked) {
			QApplication::setOverrideCursor(Qt::WaitCursor);
		} else {
			QApplication::restoreOverrideCursor();
		}
	}

public slots:
	void on_reopen_file_changed(bool status);
	void on_restore_window_size_changed(bool status);
	void on_image_open_act();
	void on_image_next_act();
	void on_image_prev_act();
	void on_delete_current_image_act();
	void on_image_close_act();
	void on_image_raw_to_fits();
	void on_image_info_act();
	void on_exit_act();
	void on_about_act();

	void on_stretch_changed(int level);
	void on_cb_changed(int balance);
	void on_debayer_changed(uint32_t bayer_pat);
	void on_antialias_view(bool status);
	void on_statistics_show(bool enabled);

private:
	// Image viewer
	TextDialog *m_image_info_dlg;
	ImageViewer *m_imager_viewer;
	preview_image *m_preview_image;
	unsigned char *m_image_data;
	size_t m_image_size;
	char m_image_path[PATH_LEN];
	char *m_image_formrat;
	QString m_selected_filter;
	QStringList m_image_list;
};

#endif // VIEWERWINDOW_H
