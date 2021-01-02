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

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

#include <QDir>
#include <QString>
#include <QObject>

#include <utils.h>
#include <conf.h>


void get_timestamp(char *timestamp_str) {
	assert(timestamp_str != nullptr);
	struct timeval tmnow;

	gettimeofday(&tmnow, NULL);
#if defined(INDIGO_WINDOWS)
	struct tm *lt;
	time_t rawtime;
	lt = localtime((const time_t *) &(tmnow.tv_sec));
	if (lt == NULL) {
		time(&rawtime);
		lt = localtime(&rawtime);
	}
	strftime(timestamp_str, 255, "%Y-%m-%d %H:%M:%S", lt);
#else
	strftime(timestamp_str, 255, "%Y-%m-%d %H:%M:%S", localtime((const time_t *) &tmnow.tv_sec));
#endif
	snprintf(timestamp_str + strlen(timestamp_str), 255, ".%03ld", tmnow.tv_usec/1000);
}

void get_date(char *date_str) {
	assert(date_str != nullptr);
	struct timeval tmnow;

	gettimeofday(&tmnow, NULL);
#if defined(INDIGO_WINDOWS)
	struct tm *lt;
	time_t rawtime;
	lt = localtime((const time_t *) &(tmnow.tv_sec));
	if (lt == NULL) {
		time(&rawtime);
		lt = localtime(&rawtime);
	}
	strftime(date_str, 255, "%Y-%m-%d", lt);
#else
	strftime(date_str, 255, "%Y-%m-%d", localtime((const time_t *) &tmnow.tv_sec));
#endif
}

#define HALF_DAY_SECONDS 43200
void get_date_jd(char *date_str) {
	assert(date_str != nullptr);
	struct timeval tmnow;

	gettimeofday(&tmnow, NULL);
	tmnow.tv_sec -= HALF_DAY_SECONDS;
#if defined(INDIGO_WINDOWS)
	struct tm *lt;
	time_t rawtime;
	lt = localtime((const time_t *) &(tmnow.tv_sec));
	if (lt == NULL) {
		time(&rawtime);
		rawtime -= HALF_DAY_SECONDS;
		lt = localtime(&rawtime);
	}
	strftime(date_str, 255, "%Y-%m-%d", lt);
#else
	strftime(date_str, 255, "%Y-%m-%d", localtime((const time_t *) &tmnow.tv_sec));
#endif
}

void get_time(char *time_str) {
	assert(time_str != nullptr);
	struct timeval tmnow;

	gettimeofday(&tmnow, NULL);
#if defined(INDIGO_WINDOWS)
	struct tm *lt;
	time_t rawtime;
	lt = localtime((const time_t *) &(tmnow.tv_sec));
	if (lt == NULL) {
		time(&rawtime);
		lt = localtime(&rawtime);
	}
	strftime(time_str, 255, "%H:%M:%S", lt);
#else
	strftime(time_str, 255, "%H:%M:%S", localtime((const time_t *) &tmnow.tv_sec));
#endif
	snprintf(time_str + strlen(time_str), 255, ".%03ld", tmnow.tv_usec/1000);
}

void get_current_output_dir(char *output_dir) {
	assert(output_dir != nullptr);

	if (QDir::homePath().length() > 0) {
		char date_str[255] = {0};
		get_date_jd(date_str);
		QString qlocation = QDir::toNativeSeparators(QDir::homePath() + QObject::tr("/ain_data/") + QObject::tr(date_str) + QObject::tr("/"));
		QDir dir = QDir::root();
		dir.mkpath(qlocation);
		strncpy(output_dir, qlocation.toUtf8().constData(), PATH_LEN);
	} else {
		if (!getcwd(output_dir, sizeof(output_dir))) {
			output_dir[0] = '\0';
		}
	}
}