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

#ifndef _SEQUENCE_MODEL_H
#define _SEQUENCE_MODEL_H

#include <QtGui>
#include <QtWidgets>

class Batch {
	bool m_empty;
	QString m_name, m_filter, m_exposure, m_delay, m_count, m_mode, m_frame, m_focus;
public:
	Batch():
		m_empty{true},
		m_name{""},
		m_filter{""},
		m_exposure{""},
		m_delay{""},
		m_count{""},
		m_mode{""},
		m_frame{""},
		m_focus{""}
	{}

	Batch(
		const QString & name,
		const QString & filter,
		const QString & exposure,
		const QString & delay,
		const QString & count,
		const QString & mode,
		const QString & frame,
		const QString & focus
	):
		m_empty{false},
		m_name{name},
		m_filter{filter},
		m_exposure{exposure},
		m_delay{delay},
		m_count{count},
		m_mode{mode},
		m_frame{frame},
		m_focus{focus}
	{}

	QString name() const { return m_name; }
	QString filter() const { return m_filter; }
	QString exposure() const { return m_exposure; }
	QString delay() const { return m_delay; }
	QString count() const { return m_count; }
	QString mode() const { return m_mode; }
	QString frame() const { return m_frame; }
	QString focus() const { return m_focus; }

	bool is_empty() const {
		return m_empty;
	}

	void set_name(QString s) {
		 m_empty = false;
		 m_name = s;
	}
	void set_filter(QString s) {
		m_empty = false;
		m_filter = s;
	}
	void set_exposure(QString s) {
		m_empty = false;
		m_exposure = s;
	}
	void set_delay(QString s) {
		m_empty = false;
		m_delay = s;
	}
	void set_count(QString s) {
		m_empty = false;
		m_count = s;
	}
	void set_mode(QString s) {
		m_empty = false;
		m_mode = s;
	}
	void set_frame(QString s) {
		m_empty = false;
		m_frame = s;
	}
	void set_focus(QString s) {
		m_empty = false;
		m_focus = s;
	}
};

/*
| focus=XXX | execute autofocus routine with XXX seconds exposure time |
| count=XXX | set AGENT_IMAGER_BATCH.COUNT to XXX |
| exposure=XXX | set AGENT_IMAGER_BATCH.EXPOSURE to XXX |
| delay=XXX | set AGENT_IMAGER_BATCH.DELAY to XXX |
| filter=XXX | set WHEEL_SLOT.SLOT to the value corresponding to name XXX |
| mode=XXX | find and select CCD_MODE item with label XXX |
| name=XXX | set CCD_LOCAL_MODE.PREFIX to XXX |
| gain=XXX | set CCD_GAIN.GAIN to XXX |
| offset=XXX | set CCD_OFFSET.OFFSET to XXX |
| gamma=XXX | set CCD_GAMMA.GAMMA to XXX |
| temperature=XXX | set CCD_TEMPERATURE.TEMPERATURE to XXX |
| cooler=XXX | find and select CCD_COOLER item with label XXX |
| frame=XXX | find and select CCD_FRAME_TYPE item with label XXX |
| aperture=XXX | find and select DSLR_APERTURE item with label XXX |
| shutter=XXX | find and select DSLR_SHUTTER item with label XXX |
| iso=XXX | find and select DSLR_ISO item with label XXX |
*/

class SequenceModel : public QAbstractTableModel {
	QList<Batch> m_data;
public:
	SequenceModel(QObject * parent = {}) : QAbstractTableModel{parent} {}
	int rowCount(const QModelIndex &) const override { return m_data.count(); }
	int columnCount(const QModelIndex &) const override { return 8; }
	QVariant data(const QModelIndex &index, int role) const override {
		if (role != Qt::DisplayRole && role != Qt::EditRole) return {};
		const auto &sequence = m_data[index.row()];
		if (sequence.is_empty()) {
			return {};
		}
		switch (index.column()) {
			case 0: return sequence.name();
			case 1: return sequence.filter();
			case 2: return sequence.exposure();
			case 3: return sequence.delay();
			case 4: return sequence.count();
			case 5: return sequence.mode();
			case 6: return sequence.frame();
			case 7: return sequence.focus();
			default: return {};
		};
	}

	QVariant headerData(int section, Qt::Orientation orientation, int role) const override {
		if ((orientation != Qt::Horizontal && orientation != Qt::Vertical) || role != Qt::DisplayRole) return {};
		if (orientation == Qt::Horizontal) {
			switch (section) {
				case 0: return "Name";
				case 1: return "Filter";
				case 2: return "Exposure";
				case 3: return "Delay";
				case 4: return "Count";
				case 5: return "Mode";
				case 6: return "Frame";
				case 7: return "Focus";
				default: return {};
			}
		}
		if (orientation == Qt::Vertical) {
			return section;
		}
	}

	void append(const Batch & batch) {
		beginInsertRows({}, m_data.count(), m_data.count());
		m_data.append(batch);
		endInsertRows();
	}

	void set_batch(const Batch & batch, int pos = -1) {
		if (pos < 0) {
			beginInsertRows({}, m_data.count(), m_data.count());
			m_data.append(batch);
			endInsertRows();
		} else {
			m_data.replace(pos, batch);
		}
	}
	Batch get_batch(int pos) {
		return m_data.value(pos);
	}
};

class SequenceViewer : public QWidget {
   QGridLayout m_layout{this};
   QTableView m_view;
   SequenceModel m_model;
   //QSortFilterProxyModel m_proxy;
   QInputDialog m_dialog;
public:
   SequenceViewer() {
      m_layout.addWidget(&m_view, 0, 0, 1, 1);
      //m_layout.addWidget(&m_button, 1, 0, 1, 1);
      //connect(&m_button, SIGNAL(clicked()), &m_dialog, SLOT(open()));
      m_model.set_batch({"M13", "800x600", "5", "Lum", "FITS","","",""});
	  m_model.set_batch({"M13", "800x600", "5", "R", "FITS","","",""});
	  m_model.append({"M13", "800x600", "5", "G", "FITS","","",""});
	  m_model.append({"M13", "800x600", "5", "B", "FITS","","",""});

	  m_model.set_batch({"M14", "800x600", "5", "R", "FITS","","",""}, 1);
      //m_proxy.setSourceModel(&m_model);
      //m_proxy.setFilterKeyColumn(2);
      m_view.setModel(&m_model);


	  m_model.set_batch({"M15", "800x600", "5", "R", "FITS","","",""}, 0);


	  Batch b = m_model.get_batch(1);
	  b.set_frame("XXXXXXXX");
	  m_model.set_batch(b);
	  //m_dialog.setLabelText("Enter registration number fragment to filter on. Leave empty to clear filter.");
	  //m_dialog.setInputMode(QInputDialog::TextInput);
	  //connect(&m_dialog, SIGNAL(textValueSelected(QString)),
	  //&m_proxy, SLOT(setFilterFixedString(QString)));
   }
};

#endif /* _SEQUENCE_MODEL_H */
