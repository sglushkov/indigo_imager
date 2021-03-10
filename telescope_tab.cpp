// Copyright (c) 2020 Rumen G.Bogdanovski
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

#include "imagerwindow.h"
#include "propertycache.h"
#include "conf.h"
#include <QLCDNumber>

#include <image_preview_lut.h>

void write_conf();

void ImagerWindow::create_telescope_tab(QFrame *telescope_frame) {
	QGridLayout *telescope_frame_layout = new QGridLayout();
	telescope_frame_layout->setAlignment(Qt::AlignTop);
	telescope_frame_layout->setColumnStretch(0, 2);
	telescope_frame_layout->setColumnStretch(1, 2);
	telescope_frame_layout->setColumnStretch(2, 3);
	telescope_frame_layout->setColumnStretch(3, 3);

	telescope_frame->setLayout(telescope_frame_layout);
	telescope_frame->setFrameShape(QFrame::StyledPanel);
	telescope_frame->setMinimumWidth(CAMERA_FRAME_MIN_WIDTH);
	telescope_frame->setContentsMargins(0, 0, 0, 0);

	int row = 0;
	m_agent_mount_select = new QComboBox();
	telescope_frame_layout->addWidget(m_agent_mount_select, row, 0, 1, 4);
	connect(m_agent_mount_select, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ImagerWindow::on_mount_agent_selected);

	row++;
	QLabel *label = new QLabel("Mount:");
	label->setStyleSheet(QString("QLabel { font-weight: bold; }"));
	telescope_frame_layout->addWidget(label, row, 0);
	m_mount_select = new QComboBox();
	telescope_frame_layout->addWidget(m_mount_select, row, 1, 1, 3);
	connect(m_mount_select, QOverload<int>::of(&QComboBox::activated), this, &ImagerWindow::on_mount_selected);

	row++;
	QSpacerItem *spacer = new QSpacerItem(1, 10, QSizePolicy::Expanding, QSizePolicy::Maximum);
	telescope_frame_layout->addItem(spacer, row, 0, 1, 4);

	#define LCD_SIZE 24
	row++;
	label = new QLabel("RA / Dec:");
	label->setStyleSheet(QString("QLabel { font-weight: bold; }"));
	telescope_frame_layout->addWidget(label, row, 0);

	m_mount_ra_label = new QLCDNumber(13);
	m_mount_ra_label->setSegmentStyle(QLCDNumber::Flat);
	m_mount_ra_label->setMinimumHeight(LCD_SIZE);
	m_mount_ra_label->display("00: 00:00.00");
	set_ok(m_mount_ra_label);
	m_mount_ra_label->show();
	telescope_frame_layout->addWidget(m_mount_ra_label, row, 2, 1, 2);

	row++;
	m_mount_dec_label = new QLCDNumber(13);
	m_mount_dec_label->setSegmentStyle(QLCDNumber::Flat);
	m_mount_dec_label->setMinimumHeight(LCD_SIZE);
	m_mount_dec_label->display("00' 00 00.00");
	set_ok(m_mount_dec_label);
	m_mount_dec_label->show();
	telescope_frame_layout->addWidget(m_mount_dec_label, row, 2, 1, 2);

	row++;
	spacer = new QSpacerItem(1, 10, QSizePolicy::Expanding, QSizePolicy::Maximum);
	telescope_frame_layout->addItem(spacer, row, 0, 1, 4);

	row++;
	label = new QLabel("LST:");
	label->setStyleSheet(QString("QLabel { font-weight: bold; }"));
	telescope_frame_layout->addWidget(label, row, 0);

	m_mount_lst_label = new QLCDNumber(13);
	m_mount_lst_label->setSegmentStyle(QLCDNumber::Flat);
	m_mount_lst_label->setMinimumHeight(LCD_SIZE);
	m_mount_lst_label->display("00: 00:00.00");
	set_ok(m_mount_lst_label);
	m_mount_lst_label->show();
	telescope_frame_layout->addWidget(m_mount_lst_label, row, 2, 1, 2);

	//row++;
	//spacer = new QSpacerItem(1, 10, QSizePolicy::Expanding, QSizePolicy::Maximum);
	//telescope_frame_layout->addItem(spacer, row, 0, 1, 4);

	row++;
	label = new QLabel("Az / Alt:");
	label->setStyleSheet(QString("QLabel { font-weight: bold; }"));
	telescope_frame_layout->addWidget(label, row, 0);

	m_mount_az_label = new QLCDNumber(13);
	m_mount_az_label->setSegmentStyle(QLCDNumber::Flat);
	m_mount_az_label->setMinimumHeight(LCD_SIZE/2);
	m_mount_az_label->display("00' 00 00.00");
	set_ok(m_mount_az_label);
	m_mount_az_label->show();
	telescope_frame_layout->addWidget(m_mount_az_label, row, 2, 1, 1);

	m_mount_alt_label = new QLCDNumber(13);
	m_mount_alt_label->setSegmentStyle(QLCDNumber::Flat);
	m_mount_alt_label->setMinimumHeight(LCD_SIZE/2);
	m_mount_alt_label->display("00' 00 00.00");
	set_ok(m_mount_alt_label);
	m_mount_alt_label->show();
	telescope_frame_layout->addWidget(m_mount_alt_label, row, 3, 1, 1);

	row++;
	spacer = new QSpacerItem(1, 10, QSizePolicy::Expanding, QSizePolicy::Maximum);
	telescope_frame_layout->addItem(spacer, row, 0, 1, 4);

	row++;
	label = new QLabel("Ra / Dec input:");
	label->setStyleSheet(QString("QLabel { font-weight: bold; }"));
	telescope_frame_layout->addWidget(label, row, 0, 1, 2);

	m_mount_ra_input = new QLineEdit();
	telescope_frame_layout->addWidget(m_mount_ra_input, row, 2);

	m_mount_dec_input = new QLineEdit();
	telescope_frame_layout->addWidget(m_mount_dec_input, row, 3);

	row++;
	QWidget *toolbar = new QWidget;
	QHBoxLayout *toolbox = new QHBoxLayout(toolbar);
	toolbar->setContentsMargins(1,1,1,1);
	toolbox->setContentsMargins(1,1,1,1);
	telescope_frame_layout->addWidget(toolbar, row, 0, 1, 4);

	m_mount_goto_button = new QPushButton("Goto");
	m_mount_goto_button->setStyleSheet("min-width: 30px");
	m_mount_goto_button->setIcon(QIcon(":resource/play.png"));
	toolbox->addWidget(m_mount_goto_button);
	connect(m_mount_goto_button, &QPushButton::clicked, this, &ImagerWindow::on_mount_goto);

	m_mount_sync_button = new QPushButton("Sync");
	m_mount_sync_button->setStyleSheet("min-width: 30px");
	m_mount_sync_button->setIcon(QIcon(":resource/calibrate.png"));
	toolbox->addWidget(m_mount_sync_button);
	connect(m_mount_sync_button , &QPushButton::clicked, this, &ImagerWindow::on_mount_sync);

	m_mount_abort_button = new QPushButton("Abort");
	m_mount_abort_button->setStyleSheet("min-width: 30px");
	m_mount_abort_button->setIcon(QIcon(":resource/stop.png"));
	toolbox->addWidget(m_mount_abort_button);
	connect(m_mount_abort_button, &QPushButton::clicked, this, &ImagerWindow::on_mount_abort);

	row++;
	// Tools tabbar
	QTabWidget *telescope_tabbar = new QTabWidget;
	telescope_frame_layout->addWidget(telescope_tabbar, row, 0, 1, 4);

	// image frame
	QFrame *slew_frame = new QFrame();
	telescope_tabbar->addTab(slew_frame, "Slew&&Track");

	QGridLayout *slew_frame_layout = new QGridLayout();
	slew_frame_layout->setAlignment(Qt::AlignTop);
	slew_frame_layout->setColumnStretch(0, 1);
	slew_frame_layout->setColumnStretch(1, 1);
	slew_frame_layout->setColumnStretch(2, 1);
	slew_frame_layout->setColumnStretch(3, 1);
	slew_frame_layout->setColumnStretch(4, 3);
	slew_frame->setLayout(slew_frame_layout);
	slew_frame->setFrameShape(QFrame::StyledPanel);
	slew_frame->setContentsMargins(0, 0, 0, 0);

	int slew_row = 0;
	int slew_col = 0;
	QPushButton *move_button = new QPushButton("N");
	move_button->setStyleSheet("min-width: 20px");
	slew_frame_layout->addWidget(move_button, slew_row + 0, slew_col + 1);
	connect(move_button, &QPushButton::pressed, this, &ImagerWindow::on_mount_move_north);
	connect(move_button, &QPushButton::released, this, &ImagerWindow::on_mount_stop_north);

	move_button = new QPushButton("W");
	move_button->setStyleSheet("min-width: 20px");
	slew_frame_layout->addWidget(move_button, slew_row + 1, slew_col + 0);
	connect(move_button, &QPushButton::pressed, this, &ImagerWindow::on_mount_move_west);
	connect(move_button, &QPushButton::released, this, &ImagerWindow::on_mount_stop_west);

	move_button = new QPushButton("");
	move_button->setStyleSheet("min-width: 20px");
	move_button->setIcon(QIcon(":resource/stop.png"));
	slew_frame_layout->addWidget(move_button, slew_row + 1, slew_col + 1);
	connect(move_button, &QPushButton::clicked, this, &ImagerWindow::on_mount_abort);

	move_button = new QPushButton("E");
	move_button->setStyleSheet("min-width: 20px");
	slew_frame_layout->addWidget(move_button, slew_row + 1, slew_col + 2);
	connect(move_button, &QPushButton::pressed, this, &ImagerWindow::on_mount_move_east);
	connect(move_button, &QPushButton::released, this, &ImagerWindow::on_mount_stop_east);

	move_button = new QPushButton("S");
	move_button->setStyleSheet("min-width: 20px");
	slew_frame_layout->addWidget(move_button, slew_row + 2, slew_col + 1);
	connect(move_button, &QPushButton::pressed, this, &ImagerWindow::on_mount_move_south);
	connect(move_button, &QPushButton::released, this, &ImagerWindow::on_mount_stop_south);

	slew_col = 4;
	slew_row = 0;
	m_mount_guide_rate_cbox = new QCheckBox("Guide rate");
	m_mount_guide_rate_cbox->setEnabled(false);
	slew_frame_layout->addWidget(m_mount_guide_rate_cbox, slew_row, slew_col);
	connect(m_mount_guide_rate_cbox, &QPushButton::clicked, this, &ImagerWindow::on_mount_set_guide_rate);

	slew_row++;
	m_mount_center_rate_cbox = new QCheckBox("Centering rate");
	m_mount_center_rate_cbox->setEnabled(false);
	slew_frame_layout->addWidget(m_mount_center_rate_cbox, slew_row, slew_col);
	connect(m_mount_center_rate_cbox, &QPushButton::clicked, this, &ImagerWindow::on_mount_set_center_rate);

	slew_row++;
	m_mount_find_rate_cbox = new QCheckBox("Finding rate");
	m_mount_find_rate_cbox->setEnabled(false);
	slew_frame_layout->addWidget(m_mount_find_rate_cbox, slew_row, slew_col);
	connect(m_mount_find_rate_cbox, &QPushButton::clicked, this, &ImagerWindow::on_mount_set_find_rate);

	slew_row++;
	m_mount_max_rate_cbox = new QCheckBox("Max rate");
	m_mount_max_rate_cbox->setEnabled(false);
	slew_frame_layout->addWidget(m_mount_max_rate_cbox, slew_row, slew_col);
	connect(m_mount_max_rate_cbox, &QPushButton::clicked, this, &ImagerWindow::on_mount_set_max_rate);

	slew_col = 0;
	slew_row++;
	m_mount_track_cbox = new QCheckBox("Tracking");
	m_mount_track_cbox->setEnabled(false);
	set_ok(m_mount_track_cbox);
	slew_frame_layout->addWidget(m_mount_track_cbox, slew_row, slew_col, 1, 3);
	connect(m_mount_track_cbox, &QPushButton::clicked, this, &ImagerWindow::on_mount_track);

	slew_row++;
	m_mount_park_cbox = new QCheckBox("Park");
	m_mount_park_cbox->setEnabled(false);
	set_ok(m_mount_park_cbox);
	slew_frame_layout->addWidget(m_mount_park_cbox, slew_row, slew_col, 1, 3);
	connect(m_mount_park_cbox, &QCheckBox::clicked, this, &ImagerWindow::on_mount_park);

	QFrame *solve_frame = new QFrame();
	telescope_tabbar->addTab(solve_frame, "Solve");

	QGridLayout *solve_frame_layout = new QGridLayout();
	solve_frame_layout->setAlignment(Qt::AlignTop);
	solve_frame->setLayout(solve_frame_layout);
	solve_frame->setFrameShape(QFrame::StyledPanel);
	solve_frame->setContentsMargins(0, 0, 0, 0);

	int solve_row = 0;

	QFrame *site_frame = new QFrame();
	telescope_tabbar->addTab(site_frame, "Site");

	QGridLayout *site_frame_layout = new QGridLayout();
	site_frame_layout->setAlignment(Qt::AlignTop);
	site_frame->setLayout(site_frame_layout);
	site_frame->setFrameShape(QFrame::StyledPanel);
	site_frame->setContentsMargins(0, 0, 0, 0);

	int site_row = 0;

}

void ImagerWindow::on_mount_agent_selected(int index) {
	QtConcurrent::run([=]() {
		// Clear controls
		indigo_property *property = (indigo_property*)malloc(sizeof(indigo_property));
		memset(property, 0, sizeof(indigo_property));
		get_selected_mount_agent(property->device);
		property_delete(property, nullptr);
		free(property);

		indigo_enumerate_properties(nullptr, &INDIGO_ALL_PROPERTIES);
	});
}

void ImagerWindow::on_mount_selected(int index) {
	QtConcurrent::run([=]() {
		static char selected_mount[INDIGO_NAME_SIZE], selected_agent[INDIGO_NAME_SIZE];
		QString q_mount_str = m_mount_select->currentText();
		if (q_mount_str.compare("No mount") == 0) {
			strcpy(selected_mount, "NONE");
		} else {
			strncpy(selected_mount, q_mount_str.toUtf8().constData(), INDIGO_NAME_SIZE);
		}
		get_selected_mount_agent(selected_agent);

		indigo_debug("[SELECTED] %s '%s' '%s'\n", __FUNCTION__, selected_agent, selected_mount);
		static const char * items[] = { selected_mount };

		static bool values[] = { true };
		indigo_change_switch_property(nullptr, selected_agent, FILTER_MOUNT_LIST_PROPERTY_NAME, 1, items, values);
	});
}

void ImagerWindow::on_mount_goto(int index) {
	QtConcurrent::run([=]() {
		static char selected_mount[INDIGO_NAME_SIZE], selected_agent[INDIGO_NAME_SIZE];
		QString q_mount_str = m_mount_select->currentText();
		if (q_mount_str.compare("No mount") == 0) {
			strcpy(selected_mount, "NONE");
		} else {
			strncpy(selected_mount, q_mount_str.toUtf8().constData(), INDIGO_NAME_SIZE);
		}
		get_selected_mount_agent(selected_agent);

		indigo_debug("[SELECTED] %s '%s' '%s'\n", __FUNCTION__, selected_agent, selected_mount);

		change_mount_agent_equatorial(selected_agent, false);
	});
}

void ImagerWindow::on_mount_sync(int index) {
	QtConcurrent::run([=]() {
		static char selected_mount[INDIGO_NAME_SIZE], selected_agent[INDIGO_NAME_SIZE];
		QString q_mount_str = m_mount_select->currentText();
		if (q_mount_str.compare("No mount") == 0) {
			strcpy(selected_mount, "NONE");
		} else {
			strncpy(selected_mount, q_mount_str.toUtf8().constData(), INDIGO_NAME_SIZE);
		}
		get_selected_mount_agent(selected_agent);

		indigo_debug("[SELECTED] %s '%s' '%s'\n", __FUNCTION__, selected_agent, selected_mount);

		change_mount_agent_equatorial(selected_agent, true);
	});
}

void ImagerWindow::on_mount_abort(int index) {
	QtConcurrent::run([=]() {
		static char selected_mount[INDIGO_NAME_SIZE], selected_agent[INDIGO_NAME_SIZE];
		QString q_mount_str = m_mount_select->currentText();
		if (q_mount_str.compare("No mount") == 0) {
			strcpy(selected_mount, "NONE");
		} else {
			strncpy(selected_mount, q_mount_str.toUtf8().constData(), INDIGO_NAME_SIZE);
		}
		get_selected_mount_agent(selected_agent);

		indigo_debug("[SELECTED] %s '%s' '%s'\n", __FUNCTION__, selected_agent, selected_mount);

		change_mount_agent_abort(selected_agent);
	});
}

void ImagerWindow::on_mount_track(int state) {
	QtConcurrent::run([=]() {
		static char selected_mount[INDIGO_NAME_SIZE], selected_agent[INDIGO_NAME_SIZE];
		QString q_mount_str = m_mount_select->currentText();
		if (q_mount_str.compare("No mount") == 0) {
			strcpy(selected_mount, "NONE");
		} else {
			strncpy(selected_mount, q_mount_str.toUtf8().constData(), INDIGO_NAME_SIZE);
		}
		get_selected_mount_agent(selected_agent);
		bool checked = m_mount_track_cbox->checkState();

		indigo_debug("[SELECTED] %s '%s' '%s'\n", __FUNCTION__, selected_agent, selected_mount);

		if (checked) {
			indigo_change_switch_property_1(nullptr, selected_agent, MOUNT_TRACKING_PROPERTY_NAME, MOUNT_TRACKING_ON_ITEM_NAME, true);
		} else {
			indigo_change_switch_property_1(nullptr, selected_agent, MOUNT_TRACKING_PROPERTY_NAME, MOUNT_TRACKING_OFF_ITEM_NAME, true);
		}
	});
}

void ImagerWindow::on_mount_park(int state) {
	QtConcurrent::run([=]() {
		static char selected_mount[INDIGO_NAME_SIZE], selected_agent[INDIGO_NAME_SIZE];
		QString q_mount_str = m_mount_select->currentText();
		if (q_mount_str.compare("No mount") == 0) {
			strcpy(selected_mount, "NONE");
		} else {
			strncpy(selected_mount, q_mount_str.toUtf8().constData(), INDIGO_NAME_SIZE);
		}
		get_selected_mount_agent(selected_agent);
		bool checked = m_mount_park_cbox->checkState();

		indigo_debug("[SELECTED] %s '%s' '%s'\n", __FUNCTION__, selected_agent, selected_mount);

		if (checked) {
			indigo_change_switch_property_1(nullptr, selected_agent, MOUNT_PARK_PROPERTY_NAME, MOUNT_PARK_PARKED_ITEM_NAME, true);
		} else {
			indigo_change_switch_property_1(nullptr, selected_agent, MOUNT_PARK_PROPERTY_NAME, MOUNT_PARK_UNPARKED_ITEM_NAME, true);
		}
	});
}

void ImagerWindow::mount_agent_set_switch_async(char *property, char *item, bool move) {
	QtConcurrent::run([=]() {
		static char selected_mount[INDIGO_NAME_SIZE], selected_agent[INDIGO_NAME_SIZE];
		QString q_mount_str = m_mount_select->currentText();
		if (q_mount_str.compare("No mount") == 0) {
			strcpy(selected_mount, "NONE");
		} else {
			strncpy(selected_mount, q_mount_str.toUtf8().constData(), INDIGO_NAME_SIZE);
		}
		get_selected_mount_agent(selected_agent);

		indigo_debug("[SELECTED] %s '%s' '%s'\n", __FUNCTION__, selected_agent, selected_mount);

		indigo_change_switch_property_1(nullptr, selected_agent, property, item, move);
	});
}

void ImagerWindow::on_mount_move_north() {
	mount_agent_set_switch_async(MOUNT_MOTION_DEC_PROPERTY_NAME, MOUNT_MOTION_NORTH_ITEM_NAME, true);
}

void ImagerWindow::on_mount_stop_north() {
	mount_agent_set_switch_async(MOUNT_MOTION_DEC_PROPERTY_NAME, MOUNT_MOTION_NORTH_ITEM_NAME, false);
}

void ImagerWindow::on_mount_move_south() {
	mount_agent_set_switch_async(MOUNT_MOTION_DEC_PROPERTY_NAME, MOUNT_MOTION_SOUTH_ITEM_NAME, true);
}

void ImagerWindow::on_mount_stop_south() {
	mount_agent_set_switch_async(MOUNT_MOTION_DEC_PROPERTY_NAME, MOUNT_MOTION_SOUTH_ITEM_NAME, false);
}

void ImagerWindow::on_mount_move_east() {
	mount_agent_set_switch_async(MOUNT_MOTION_RA_PROPERTY_NAME, MOUNT_MOTION_EAST_ITEM_NAME, true);
}

void ImagerWindow::on_mount_stop_east() {
	mount_agent_set_switch_async(MOUNT_MOTION_RA_PROPERTY_NAME, MOUNT_MOTION_EAST_ITEM_NAME, false);
}

void ImagerWindow::on_mount_move_west() {
	mount_agent_set_switch_async(MOUNT_MOTION_RA_PROPERTY_NAME, MOUNT_MOTION_WEST_ITEM_NAME, true);
}

void ImagerWindow::on_mount_stop_west() {
	mount_agent_set_switch_async(MOUNT_MOTION_RA_PROPERTY_NAME, MOUNT_MOTION_WEST_ITEM_NAME, false);
}

void ImagerWindow::on_mount_set_guide_rate(int state) {
	mount_agent_set_switch_async(MOUNT_SLEW_RATE_PROPERTY_NAME, MOUNT_SLEW_RATE_GUIDE_ITEM_NAME, true);
}

void ImagerWindow::on_mount_set_center_rate(int state) {
	mount_agent_set_switch_async(MOUNT_SLEW_RATE_PROPERTY_NAME, MOUNT_SLEW_RATE_CENTERING_ITEM_NAME, true);
}

void ImagerWindow::on_mount_set_find_rate(int state) {
	mount_agent_set_switch_async(MOUNT_SLEW_RATE_PROPERTY_NAME, MOUNT_SLEW_RATE_FIND_ITEM_NAME, true);
}

void ImagerWindow::on_mount_set_max_rate(int state) {
	mount_agent_set_switch_async(MOUNT_SLEW_RATE_PROPERTY_NAME, MOUNT_SLEW_RATE_MAX_ITEM_NAME, true);
}