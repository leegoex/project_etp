from django.db import models
from django import forms

class Crossing(models.Model):
	serial = models.CharField(max_length=30)
	code = models.CharField(max_length=30)
	name = models.CharField(max_length=30)
	kilometer = models.CharField(max_length=30)
	meter = models.CharField(max_length=30)
	operator = models.CharField(max_length=30)
	name_format = models.CharField(max_length=30)
	image_size = models.CharField(max_length=30)
	image_quality = models.CharField(max_length=30)

	def Update(self):
		changed = False
		item_list = Crossing.objects.all()
		if item_list:
			update_item = item_list[0]
			if update_item.serial != self.serial:
				changed = True
				update_item.serial = self.serial
			if update_item.code != self.code:
				changed = True
				update_item.code = self.code
			if update_item.code != self.code:
				changed = True
				update_item.code = self.code
			if update_item.name != self.name:
				changed = True
				update_item.name = self.name
			if update_item.kilometer != self.kilometer:
				changed = True
				update_item.kilometer = self.kilometer
			if update_item.meter != self.meter:
				changed = True
				update_item.meter = self.meter
			if update_item.operator != self.operator:
				changed = True
				update_item.operator = self.operator
			if update_item.name_format != self.name_format:
				changed = True
				update_item.name_format = self.name_format
			if update_item.image_size != self.image_size:
				changed = True
				update_item.image_size = self.image_size
			if update_item.image_quality != self.image_quality:
				changed = True
				update_item.image_quality = self.image_quality
			if changed:
				update_item.save()
		else:
			changed = True
			self.save()
		return changed


class MeasureSpeed(models.Model):
	speed_limit = models.CharField(max_length=30)
	speed_starting = models.CharField(max_length=30)
	speed_delta = models.CharField(max_length=30)
	lane_same = models.CharField(max_length=30)
	enable_direction1 = models.CharField(max_length=30)
	direction1 = models.CharField(max_length=30)
	d1_lane1_delta = models.CharField(max_length=30)
	d1_lane2_delta = models.CharField(max_length=30)
	d1_lane3_delta = models.CharField(max_length=30)
	d1_lane4_delta = models.CharField(max_length=30)
	enable_direction2 = models.CharField(max_length=30)
	direction2 = models.CharField(max_length=30)
	d2_lane1_delta = models.CharField(max_length=30)
	d2_lane2_delta = models.CharField(max_length=30)
	d2_lane3_delta = models.CharField(max_length=30)
	d2_lane4_delta = models.CharField(max_length=30)

	def Update(self):
		item_list = MeasureSpeed.objects.all()
		if item_list:
			changed = False
			update_item = item_list[0]
			if update_item.speed_limit != self.speed_limit:
				changed = True
				update_item.speed_limit = self.speed_limit
			if update_item.speed_starting != self.speed_starting:
				changed = True
				update_item.speed_starting = self.speed_starting
			if update_item.speed_delta != self.speed_delta:
				changed = True
				update_item.speed_delta = self.speed_delta
			if update_item.lane_same != self.lane_same:
				changed = True
				update_item.lane_same = self.lane_same
			if update_item.enable_direction1 != self.enable_direction1:
				changed = True
				update_item.enable_direction1 = self.enable_direction1
			if update_item.direction1 != self.direction1:
				changed = True
				update_item.direction1 = self.direction1
			if update_item.d1_lane1_delta != self.d1_lane1_delta:
				changed = True
				update_item.d1_lane1_delta = self.d1_lane1_delta
			if update_item.d1_lane2_delta != self.d1_lane2_delta:
				changed = True
				update_item.d1_lane2_delta = self.d1_lane2_delta
			if update_item.d1_lane3_delta != self.d1_lane3_delta:
				changed = True
				update_item.d1_lane3_delta = self.d1_lane3_delta
			if update_item.d1_lane4_delta != self.d1_lane4_delta:
				changed = True
				update_item.d1_lane4_delta = self.d1_lane4_delta
			if update_item.enable_direction2 != self.enable_direction2:
				changed = True
				update_item.enable_direction2 = self.enable_direction2
			if update_item.direction2 != self.direction2:
				changed = True
				update_item.direction2 = self.direction2
			if update_item.d2_lane1_delta != self.d2_lane1_delta:
				changed = True
				update_item.d2_lane1_delta = self.d2_lane1_delta
			if update_item.d2_lane2_delta != self.d2_lane2_delta:
				changed = True
				update_item.d2_lane2_delta = self.d2_lane2_delta
			if update_item.d2_lane3_delta != self.d2_lane3_delta:
				changed = True
				update_item.d2_lane3_delta = self.d2_lane3_delta
			if update_item.d2_lane4_delta != self.d2_lane4_delta:
				changed = True
				update_item.d2_lane4_delta = self.d2_lane4_delta
			if changed:
				update_item.save()
		else:
			changed = True
			self.save()
		return changed


class FTPSetting(models.Model):
	enable_ftp = models.CharField(max_length=30)
	remote = models.CharField(max_length=30)
	port = models.CharField(max_length=30)
	enable_time = models.CharField(max_length=30)
	time_start = models.CharField(max_length=30)
	time_finished = models.CharField(max_length=30)
	username = models.CharField(max_length=30)
	password = models.CharField(max_length=30)

	def Update(self):
		changed = False
		item_list = FTPSetting.objects.all()
		if item_list:
			update_item = item_list[0]
			if update_item.enable_ftp != self.enable_ftp:
				changed = True
				update_item.enable_ftp = self.enable_ftp
			if update_item.remote != self.remote:
				changed = True
				update_item.remote = self.remote
			if update_item.port != self.port:
				changed = True
				update_item.port = self.port
			if update_item.enable_time != self.enable_time:
				changed = True
				update_item.enable_time = self.enable_time
			if update_item.time_start != self.time_start:
				changed = True
				update_item.time_start = self.time_start
			if update_item.time_finished != self.time_finished:
				changed = True
				update_item.time_finished = self.time_finished
			if update_item.username != self.username:
				changed = True
				update_item.username = self.username
			if update_item.password != self.password:
				changed = True
				update_item.password = self.password
			if changed:
				update_item.save()
		else:
			changed = True
			self.save()
		return changed


class ServerUpload(models.Model):
	enable_upload = models.CharField(max_length=30)
	serverip = models.CharField(max_length=30)
	serverport = models.CharField(max_length=30)

	def Update(self):
		changed = False
		item_list = ServerUpload.objects.all()
		if item_list:
			update_item = item_list[0]
			if update_item.enable_upload != self.enable_upload:
				changed = True
				update_item.enable_upload = self.enable_upload
			if update_item.serverip != self.serverip:
				changed = True
				update_item.serverip = self.serverip
			if update_item.serverport != self.serverport:
				changed = True
				update_item.serverport = self.serverport
			if changed:
				update_item.save()
		else:
			changed = True
			self.save()
		return changed


class ClearSetting(models.Model):
	elapse = models.CharField(max_length=30)
	enable_usb = models.CharField(max_length=30)

	def Update(self):
		changed = False
		item_list = ClearSetting.objects.all()
		if item_list:
			update_item = item_list[0]
			if update_item.elapse != self.elapse:
				changed = True
				update_item.elapse = self.elapse
			if update_item.enable_usb != self.enable_usb:
				changed = True
				update_item.enable_usb = self.enable_usb
			if changed:
				update_item.save()
		else:
			changed = True
			self.save()
		return changed


class Networking(models.Model):
	ipaddress = models.CharField(max_length=30)
	netmask = models.CharField(max_length=30)
	gateway = models.CharField(max_length=30)
	dns = models.CharField(max_length=30)

	def Update(self):
		changed = False
		item_list = Networking.objects.all()
		if item_list:
			update_item = item_list[0]
			if update_item.ipaddress != self.ipaddress:
				changed = True
				update_item.ipaddress = self.ipaddress
			if update_item.netmask != self.netmask:
				changed = True
				update_item.netmask = self.netmask
			if update_item.gateway != self.gateway:
				changed = True
				update_item.gateway = self.gateway
			if update_item.dns != self.dns:
				changed = True
				update_item.dns = self.dns
			if changed:
				update_item.save()
		else:
			changed = True
			self.save()
		return changed


class Datetime(models.Model):
	date = models.CharField(max_length=30)
	time = models.CharField(max_length=30)
	enable_sync = models.CharField(max_length=30)
	time_server = models.CharField(max_length=30)

	def Update(self):
		changed = False
		item_list = Datetime.objects.all()
		if item_list:
			update_item = item_list[0]
			if update_item.date != self.date:
				changed = True
				update_item.date = self.date
			if update_item.time != self.time:
				changed = True
				update_item.time = self.time
			if update_item.enable_sync != self.enable_sync:
				changed = True
				update_item.enable_sync = self.enable_sync
			if update_item.time_server != self.time_server:
				changed = True
				update_item.time_server = self.time_server
			if changed:
				update_item.save()
		else:
			changed = True
			self.save()
		return changed


class Login(models.Model):
	username = models.CharField(max_length=30)
	password = models.CharField(max_length=30)

	def Update(self):
		changed = False
		item_list = Login.objects.all()
		if item_list:
			update_item = item_list[0]
			if update_item.username != self.username:
				changed = True
				update_item.username = self.username
			if update_item.password != self.password:
				changed = True
				update_item.password = self.password
			if changed:
				update_item.save()
		else:
			changed = True
			self.save()
		return changed


class LogMessage(models.Model):
	datetime = models.CharField(max_length=30)
	message = models.CharField(max_length=30)
