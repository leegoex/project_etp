# -*- coding: utf-8 -*-

from django.template import Context, loader, RequestContext
from django.http import HttpResponse
from django.core.servers.basehttp import FileWrapper
from django.conf import settings
from django.contrib.auth import authenticate, login, logout
from django.shortcuts import redirect
from django.contrib.auth.models import User
from etpapp.models import Crossing, MeasureSpeed, FTPSetting, ServerUpload, ClearSetting, Networking, Datetime, Login

import os
import statvfs
import time
import tempfile, zipfile
import mimetypes
import xml.etree.ElementTree as xmlet
import subprocess 

label_list = [
	{'url':'/runstatus/','text':'运行状态'},
	{'url':'/basicsettings/','text':'基本参数'},
	{'url':'/syssettings/','text':'系统参数'},
	{'url':'/log/','text':'日志'},
	{'url':'/logout/','text':'退出'},
]

name_format_list = [
	{'value':'0','text':'1-->标准1'},
	{'value':'1','text':'2-->标准2'},
	{'value':'2','text':'3-->标准3'},
	{'value':'3','text':'4-->标准4(东莞)'},
	{'value':'4','text':'5-->标准5'},
]
image_size_list = [
	{'value':'0','text':'1024*800'},
	{'value':'1','text':'1280*1024(130万)'},
	{'value':'2','text':'1920*1080(200万)'},
	{'value':'3','text':'1600*1200(200万)'},
	{'value':'4','text':'2048*1536(300万)'},
	{'value':'5','text':'2592*1944(500万)'},
]
direction_list = [
	{'value':'0','text':'由东向西'},
	{'value':'1','text':'由南向北'},
	{'value':'2','text':'由西向东'},
	{'value':'3','text':'由北向南'},
]
time_list = [
	{'value':'0','text':'00'},
	{'value':'1','text':'01'},
	{'value':'2','text':'02'},
	{'value':'3','text':'03'},
	{'value':'4','text':'04'},
	{'value':'5','text':'05'},
	{'value':'6','text':'06'},
	{'value':'7','text':'07'},
	{'value':'8','text':'08'},
	{'value':'9','text':'09'},
	{'value':'10','text':'10'},
	{'value':'11','text':'11'},
	{'value':'12','text':'12'},
	{'value':'13','text':'13'},
	{'value':'14','text':'14'},
	{'value':'15','text':'15'},
	{'value':'16','text':'16'},
	{'value':'17','text':'17'},
	{'value':'18','text':'18'},
	{'value':'19','text':'19'},
	{'value':'20','text':'20'},
	{'value':'21','text':'21'},
	{'value':'22','text':'22'},
	{'value':'23','text':'23'},
]

network_path = "/etc/network/interfaces"
network_format = """
auto lo
iface lo inet loopback

allow-hotplug eth0

iface eth0 inet static
address %s 
netmask %s
gateway %s

dns-nameservers %s
"""

dns_path = "/etc/resolv.conf"
dns_format = """
nameserver %s
"""

def xml_indent(elem, level=0):
	i = "\n" + level * "\t"
	if len(elem):
		if not elem.text or not elem.text.strip():
			elem.text = i + "\t"
		for e in elem:
		 	xml_indent(e, level+1)
		if not e.tail or not e.tail.strip():
			e.tail = i
	if level and (not elem.tail or not elem.tail.strip()):
		elem.tail = i
	return elem

def generate_settings(filename):
	settings = xmlet.ElementTree()

	root = xmlet.Element("settings")
	settings._setroot(root)
	basic = xmlet.SubElement(root, "basicsettings")
	# 路口参数
	crossing = xmlet.SubElement(basic, "crossing")
	xmlet.SubElement(crossing, "serial").text = Crossing.objects.all()[0].serial
	xmlet.SubElement(crossing, "code").text = Crossing.objects.all()[0].code
	xmlet.SubElement(crossing, "name").text = Crossing.objects.all()[0].name
	xmlet.SubElement(crossing, "kilometer").text = Crossing.objects.all()[0].kilometer
	xmlet.SubElement(crossing, "meter").text = Crossing.objects.all()[0].meter
	xmlet.SubElement(crossing, "operator").text = Crossing.objects.all()[0].operator
	xmlet.SubElement(crossing, "name_format").text = Crossing.objects.all()[0].name_format
	xmlet.SubElement(crossing, "image_size").text = Crossing.objects.all()[0].image_size
	xmlet.SubElement(crossing, "image_quality").text = Crossing.objects.all()[0].image_quality
	# 测速参数
	speed = xmlet.SubElement(basic, "speed")
	xmlet.SubElement(speed, "speed_limit").text = MeasureSpeed.objects.all()[0].speed_limit
	xmlet.SubElement(speed, "speed_starting").text = MeasureSpeed.objects.all()[0].speed_starting
	xmlet.SubElement(speed, "speed_delta").text = MeasureSpeed.objects.all()[0].speed_delta
	xmlet.SubElement(speed, "lane_same").text = MeasureSpeed.objects.all()[0].lane_same
	xmlet.SubElement(speed, "enable_direction1").text = MeasureSpeed.objects.all()[0].enable_direction1
	xmlet.SubElement(speed, "direction1").text = MeasureSpeed.objects.all()[0].direction1
	xmlet.SubElement(speed, "d1_lane1_delta").text = MeasureSpeed.objects.all()[0].d1_lane1_delta
	xmlet.SubElement(speed, "d1_lane2_delta").text = MeasureSpeed.objects.all()[0].d1_lane2_delta
	xmlet.SubElement(speed, "d1_lane3_delta").text = MeasureSpeed.objects.all()[0].d1_lane3_delta
	xmlet.SubElement(speed, "d1_lane4_delta").text = MeasureSpeed.objects.all()[0].d1_lane4_delta
	xmlet.SubElement(speed, "enable_direction2").text = MeasureSpeed.objects.all()[0].enable_direction2
	xmlet.SubElement(speed, "direction2").text = MeasureSpeed.objects.all()[0].direction2
	xmlet.SubElement(speed, "d2_lane1_delta").text = MeasureSpeed.objects.all()[0].d2_lane1_delta
	xmlet.SubElement(speed, "d2_lane2_delta").text = MeasureSpeed.objects.all()[0].d2_lane2_delta
	xmlet.SubElement(speed, "d2_lane3_delta").text = MeasureSpeed.objects.all()[0].d2_lane3_delta
	xmlet.SubElement(speed, "d2_lane4_delta").text = MeasureSpeed.objects.all()[0].d2_lane4_delta
	# FTP参数
	ftp = xmlet.SubElement(basic, "ftp")
	xmlet.SubElement(ftp, "enable_ftp").text = FTPSetting.objects.all()[0].enable_ftp
	xmlet.SubElement(ftp, "remote").text = FTPSetting.objects.all()[0].remote
	xmlet.SubElement(ftp, "port").text = FTPSetting.objects.all()[0].port
	xmlet.SubElement(ftp, "enable_time").text = FTPSetting.objects.all()[0].enable_time
	xmlet.SubElement(ftp, "time_start").text = FTPSetting.objects.all()[0].time_start
	xmlet.SubElement(ftp, "time_finished").text = FTPSetting.objects.all()[0].time_finished
	xmlet.SubElement(ftp, "username").text = FTPSetting.objects.all()[0].username
	xmlet.SubElement(ftp, "password").text = FTPSetting.objects.all()[0].password
	# 服务器上传参数
	upload = xmlet.SubElement(basic, "upload")
	xmlet.SubElement(upload, "enable_upload").text = ServerUpload.objects.all()[0].enable_upload
	xmlet.SubElement(upload, "serverip").text = ServerUpload.objects.all()[0].serverip
	xmlet.SubElement(upload, "serverport").text = ServerUpload.objects.all()[0].serverport
	# 清理参数
	clear = xmlet.SubElement(basic, "clear")
	xmlet.SubElement(clear, "elapse").text = ClearSetting.objects.all()[0].elapse
	xmlet.SubElement(clear, "enable_usb").text = ClearSetting.objects.all()[0].enable_usb

	system = xmlet.SubElement(root, "syssettings")
	# 网络参数
	networking = xmlet.SubElement(system, "networking")
	xmlet.SubElement(networking, "ipaddress").text = Networking.objects.all()[0].ipaddress
	xmlet.SubElement(networking, "netmask").text = Networking.objects.all()[0].netmask
	xmlet.SubElement(networking, "gateway").text = Networking.objects.all()[0].gateway
	xmlet.SubElement(networking, "dns").text = Networking.objects.all()[0].dns
	# 日期和时间
	datetime = xmlet.SubElement(system, "datetime")
	xmlet.SubElement(datetime, "date").text = Datetime.objects.all()[0].date
	xmlet.SubElement(datetime, "time").text = Datetime.objects.all()[0].time
	xmlet.SubElement(datetime, "enable_sync").text = Datetime.objects.all()[0].enable_sync
	xmlet.SubElement(datetime, "time_server").text = Datetime.objects.all()[0].time_server
	# 用户登录
	login = xmlet.SubElement(system, "login")
	xmlet.SubElement(login, "username").text = Login.objects.all()[0].username
	xmlet.SubElement(login, "password").text = Login.objects.all()[0].password

	xmlet.dump(xml_indent(root))
	settings.write(filename, "utf-8")

def default_settings(filename):
	settings = xmlet.parse(filename)

	root = settings.getroot()
	basic = root.find("basicsettings")
	# 路口参数
	crossing = basic.find("crossing")
	crossing_param = Crossing(
		serial=crossing.findtext('serial', ''),
		code=crossing.findtext('code', ''),
		name=crossing.findtext('name'),
		kilometer=crossing.findtext('kilometer'),
		meter=crossing.findtext('meter'),
		operator=crossing.findtext('operator', ''),
		name_format=crossing.findtext('name_format', ''),
		image_size=crossing.findtext('image_size', ''),
		image_quality=crossing.findtext('image_quality', ''),
		)
	crossing_param.Update()
	# 测速参数
	speed = basic.find("speed")
	measure_speed_param = MeasureSpeed(
		speed_limit=speed.findtext('speed_limit', ''),
		speed_starting=speed.findtext('speed_starting', ''),
		speed_delta=speed.findtext('speed_delta', ''),
		lane_same=speed.findtext('lane_same', ''),
		enable_direction1=speed.findtext('enable_direction1', ''),
		direction1=speed.findtext('direction1', ''),
		d1_lane1_delta=speed.findtext('d1_lane1_delta', ''),
		d1_lane2_delta=speed.findtext('d1_lane2_delta', ''),
		d1_lane3_delta=speed.findtext('d1_lane3_delta', ''),
		d1_lane4_delta=speed.findtext('d1_lane4_delta', ''),
		enable_direction2=speed.findtext('enable_direction2', ''),
		direction2=speed.findtext('direction2', ''),
		d2_lane1_delta=speed.findtext('d2_lane1_delta', ''),
		d2_lane2_delta=speed.findtext('d2_lane2_delta', ''),
		d2_lane3_delta=speed.findtext('d2_lane3_delta', ''),
		d2_lane4_delta=speed.findtext('d2_lane4_delta', ''),
		)
	measure_speed_param.Update()
	# FTP参数
	ftp = basic.find("ftp")
	ftp_param = FTPSetting(
		enable_ftp=ftp.findtext('enable_ftp', ''),
		remote=ftp.findtext('remote', ''),
		port=ftp.findtext('port', ''),
		enable_time=ftp.findtext('enable_time', ''),
		time_start=ftp.findtext('time_start', ''),
		time_finished=ftp.findtext('time_finished', ''),
		username=ftp.findtext('username', ''),
		password=ftp.findtext('password', ''),
		)
	ftp_param.Update()
	# 服务器上传参数
	upload = basic.find("upload")
	upload_param = ServerUpload(
		enable_upload=upload.findtext('enable_upload', ''),
		serverip=upload.findtext('serverip', ''),
		serverport=upload.findtext('serverport', ''),
		)
	upload_param.Update()
	# 清理参数
	clear = basic.find("clear")
	clear_param = ClearSetting(
		elapse=clear.findtext('elapse', ''),
		enable_usb=clear.findtext('enable_usb', ''),
		)
	clear_param.Update()

	system = root.find("syssettings")
	# 网络参数
	networking = system.find("networking")
	network_param = Networking(
		ipaddress=networking.findtext('ipaddress', ''),
		netmask=networking.findtext('netmask', ''),
		gateway=networking.findtext("gateway", ''),
		dns=networking.findtext("dns", ''),
		)
	network_param.Update()
	# 日期和时间
	datetime = system.find("datetime")
	datetime_param = Datetime(
		date=networking.findtext('date', ''),
		time=networking.findtext('time', ''),
		enable_sync=networking.findtext("enable_sync", ''),
		time_server=networking.findtext("time_server", ''),
		)
	datetime_param.Update()
	# 用户登录
	login = system.find("login")
	login_param = Login(
		username=login.findtext('username', ''),
		password=login.findtext('password', ''),
		)
	login_param.Update()
	
def runstatus(request): 
	if not request.user.is_active:
		return redirect("/login/")
	if request.method == "POST":
		todo = request.POST.get("todo", '')
		if todo == u"下载参数":
			filename = "/home/settings.xml"
			download_name ="settings.xml"
			generate_settings(filename)
			wrapper = FileWrapper(open(filename))
			content_type = mimetypes.guess_type(filename)[0]
			response = HttpResponse(wrapper,content_type=content_type)
			response['Content-Length']      = os.path.getsize(filename)    
			response['Content-Disposition'] = "attachment; filename=%s"%download_name
			return response
		elif todo == u"恢复出厂设置":
			default_settings("/home/settings.xml")
		elif todo == u"重启软件":
			print 3
		elif todo == u"重启设备":
			print 4

	main_status = "未运行"
	# 检测软件工作状态
	for line in os.popen("ps xa"):
		fields = line.split()
		if fields[4] == 'etpmain':
			main_status="运行中"
			break
	# 检测磁盘空间
	vfs = os.statvfs("/home")
	capacity = round(vfs[statvfs.F_BLOCKS]*vfs[statvfs.F_BSIZE]/(1024*1024*1024.0), 1)
	available = round(vfs[statvfs.F_BAVAIL]*vfs[statvfs.F_BSIZE]/(1024*1024*1024.0), 1)
	storage = "可用空间：%.1fGB 总空间：%.1fGB" % (available,capacity)
	# 获取系统时间
	system_datetime=time.strftime('%Y-%m-%d %H:%M:%S',time.localtime(time.time()))
	#获取本机网络参数
	networking = [] 
	file=os.popen("ifconfig | grep 'addr:'")
	data=file.read()
	file.close()
	bits=data.strip().split('\n')
	for bit in bits:
		if bit.strip().startswith("inet "):
			other_bits=bit.replace(':', ' ').strip().split(' ')
			for obit in other_bits:
				if (obit.count('.')==3):
					if not obit.startswith("127."):
						networking.append(obit)
						break
	# 获取FTP状态
	ftp_status = ""
	ftp_list = FTPSetting.objects.all()
	if ftp_list and ftp_list[0].enable_ftp=="checked":
		ftp_status = "已启用" 
	else:
		ftp_status = "未启用" 
	# 获取自动上传状态
	upload_status = ""
	upload_list = ServerUpload.objects.all()
	if upload_list and upload_list[0].enable_upload=="checked":
		upload_status = "已启用" 
	else:
		upload_status = "未启用" 

	t = loader.get_template('runstatus.html')
	c = RequestContext(request, {
		'main_status':main_status,
		'storage':storage,
		'system_datetime':system_datetime,
		'networking':networking[0],
		'ftp_status':ftp_status,
		'upload_status':upload_status,
		'label_list':label_list,
		'current_url':request.path,
		})
	return HttpResponse(t.render(c))

def basicsettings(request):
	if not request.user.is_active:
		return redirect("/login/")
	if request.method == "POST":
		crossing_param = Crossing(
				serial=request.POST.get('serial', ''),
				code=request.POST.get('code', ''),
				name=request.POST.get('name', ''),
				kilometer=request.POST.get('kilometer', ''),
				meter=request.POST.get('meter', ''),
				operator=request.POST.get('operator', ''),
				name_format=request.POST.get('name_format', ''),
				image_size=request.POST.get('image_size', ''),
				image_quality=request.POST.get('image_quality', ''),
				)
		crossing_param.Update()
		measure_speed_param = MeasureSpeed(
				speed_limit=request.POST.get('speed_limit', ''),
				speed_starting=request.POST.get('speed_starting', ''),
				speed_delta=request.POST.get('speed_delta', ''),
				lane_same=request.POST.get('lane_same', ''),
				enable_direction1=request.POST.get('enable_direction1', ''),
				direction1=request.POST.get('direction1', ''),
				d1_lane1_delta=request.POST.get('d1_lane1_delta', ''),
				d1_lane2_delta=request.POST.get('d1_lane2_delta', ''),
				d1_lane3_delta=request.POST.get('d1_lane3_delta', ''),
				d1_lane4_delta=request.POST.get('d1_lane4_delta', ''),
				enable_direction2=request.POST.get('enable_direction2', ''),
				direction2=request.POST.get('direction2', ''),
				d2_lane1_delta=request.POST.get('d2_lane1_delta', ''),
				d2_lane2_delta=request.POST.get('d2_lane2_delta', ''),
				d2_lane3_delta=request.POST.get('d2_lane3_delta', ''),
				d2_lane4_delta=request.POST.get('d2_lane4_delta', ''),
				)
		measure_speed_param.Update()
		ftp_param = FTPSetting(
				enable_ftp=request.POST.get('enable_ftp', ''),
				remote=request.POST.get('remote', ''),
				port=request.POST.get('port', ''),
				enable_time=request.POST.get('enable_time', ''),
				time_start=request.POST.get('time_start', ''),
				time_finished=request.POST.get('time_finished', ''),
				username=request.POST.get('username', ''),
				password=request.POST.get('password', ''),
				)
		ftp_param.Update()
		upload_param = ServerUpload(
				enable_upload=request.POST.get('enable_upload', ''),
				serverip=request.POST.get('serverip', ''),
				serverport=request.POST.get('serverport', ''),
				)
		upload_param.Update()
		clear_param = ClearSetting(
				elapse=request.POST.get('elapse', ''),
				enable_usb=request.POST.get('enable_usb', ''),
				)
		clear_param.Update()

	crossing_list = Crossing.objects.all()
	if crossing_list:
		crossing_item = crossing_list[0]
	else:
		crossing_item = Crossing()
	speed_list = MeasureSpeed.objects.all()
	if speed_list:
		speed_item = speed_list[0]
	else:
		speed_item = MeasureSpeed()
	ftp_list = FTPSetting.objects.all()
	if ftp_list:
		ftp_item = ftp_list[0]
	else:
		ftp_item = FTPSetting()
	upload_list = ServerUpload.objects.all()
	if upload_list:
		upload_item = upload_list[0]
	else:
		upload_item = ServerUpload()
	clear_list = ClearSetting.objects.all()
	if clear_list:
		clear_item = clear_list[0]
	else:
		clear_item = ClearSetting()
	t = loader.get_template('basicsettings.html')
	c = RequestContext(request, {
		'label_list':label_list,
		'current_url':request.path,
		'serial':crossing_item.serial,
		'code':crossing_item.code,
		'name':crossing_item.name,
		'kilometer':crossing_item.kilometer,
		'meter':crossing_item.meter,
		'operator':crossing_item.operator,
		'name_format':crossing_item.name_format,
		'image_size':crossing_item.image_size,
		'image_quality':crossing_item.image_quality,
		'format_list':name_format_list,
		'image_size_list':image_size_list,

		'speed_limit':speed_item.speed_limit,
		'speed_starting':speed_item.speed_starting,
		'speed_delta':speed_item.speed_delta,
		'lane_same':speed_item.lane_same,
		'enable_direction1':speed_item.enable_direction1,
		'direction1':speed_item.direction1,
		'd1_lane1_delta':speed_item.d1_lane1_delta,
		'd1_lane2_delta':speed_item.d1_lane2_delta,
		'd1_lane3_delta':speed_item.d1_lane3_delta,
		'd1_lane4_delta':speed_item.d1_lane4_delta,
		'enable_direction2':speed_item.enable_direction2,
		'direction2':speed_item.direction2,
		'd2_lane1_delta':speed_item.d2_lane1_delta,
		'd2_lane2_delta':speed_item.d2_lane2_delta,
		'd2_lane3_delta':speed_item.d2_lane3_delta,
		'd2_lane4_delta':speed_item.d2_lane4_delta,
		'direction_list':direction_list,

		'enable_ftp':ftp_item.enable_ftp,
		'remote':ftp_item.remote,
		'port':ftp_item.port,
		'enable_time':ftp_item.enable_time,
		'time_start':ftp_item.time_start,
		'time_finished':ftp_item.time_finished,
		'username':ftp_item.username,
		'password':ftp_item.password,
		'time_list':time_list,

		'enable_upload':upload_item.enable_upload,
		'serverip':upload_item.serverip,
		'serverport':upload_item.serverport,

		'elapse':clear_item.elapse,
		'enable_usb':clear_item.enable_usb,
		})
	return HttpResponse(t.render(c))

def handle_uploaded_file(f):
	if f:
		with open("upload/"+f.name, 'wb+') as destination:
			for chunk in f.chunks():
				destination.write(chunk)
	else:
		print "No file upload"
	
def syssettings(request):
	if not request.user.is_active:
		return redirect("/login/")
	if request.method == 'POST':
		todo = request.POST.get("todo", '')
		if todo == u"升级":
			handle_uploaded_file(request.FILES.get("update_file",""))
		elif todo == u"保存网络参数":
			network_param = Networking(
				ipaddress=request.POST.get('ipaddress', ''),
				netmask=request.POST.get('netmask', ''),
				gateway=request.POST.get('gateway', ''),
				dns=request.POST.get('dns', ''),
				)
			if network_param.Update():
				f_net = open(network_path, "w")
				buf = network_format % (network_param.ipaddress, network_param.netmask, network_param.gateway, network_param.dns)
				f_net.write(buf)
				f_net.close()
				f_dns = open(dns_path, "w")
				buf = dns_format % network_param.dns
				f_dns.write(buf)
				# 重启电脑
				subprocess.call("reboot", shell=True)

		elif todo == u"保存时间设置":
			datetime_param = Datetime(
				date=request.POST.get('date', ''),
				time=request.POST.get('time', ''),
				enable_sync=request.POST.get('enable_sync', ''),
				time_server=request.POST.get('time_server', ''),
				)
			if datetime_param.Update():
				subprocess.call("date -s %s" % datetime_param.date, shell=True)
				subprocess.call("date -s %s" % datetime_param.time, shell=True)
				subprocess.call("hwclock -w", shell=True)
				if datetime_param.enable_sync == "checked":
					subprocess.call("ntpdate %s" % datetime_param.time_server, shell=True)

		elif todo == u"修改":
			login_param = Login(
				username=request.POST.get('username', ''),
				password=request.POST.get('password', ''),
				)
			if login_param.Update():
				u = User.objects.get(username=login_param.username)
				if u:
					u.set_password(login_param.password)
					u.save()
				else:
					u = User.objects.create_user(login_param.username, "abc@123.com", login_param.password)
					u.save()

	network_list = Networking.objects.all()
	if network_list:
		network_item = network_list[0]
	else:
		network_item = Networking()
	datetime_list = Datetime.objects.all()
	if datetime_list:
		datetime_item = datetime_list[0]
	else:
		datetime_item = Datetime()
	login_list = Login.objects.all()
	if login_list:
		login_item = login_list[0]
	else:
		login_item = Login()
	t = loader.get_template('syssettings.html')
	c = RequestContext(request, {
		'label_list':label_list,
		'current_url':request.path,
		'ipaddress':network_item.ipaddress,
		'netmask':network_item.netmask,
		'gateway':network_item.gateway,
		'dns':network_item.dns,
		'date':datetime_item.date,
		'time':datetime_item.time,
		'enable_sync':datetime_item.enable_sync,
		'time_server':datetime_item.time_server,
		'username':login_item.username,
		'password':login_item.password,
	})
	return HttpResponse(t.render(c))

def log(request):
	if not request.user.is_active:
		return redirect("/login/")
	t = loader.get_template('log.html')
	c = RequestContext(request, {
		'label_list':label_list,
		'current_url':request.path,
		})
	return HttpResponse(t.render(c))

def signin(request):
	if request.method == "POST":
		username=request.POST.get('username', '')
		password=request.POST.get('password', '')
		user = authenticate(username=username, password=password)
		if user is not None:
			if user.is_active:
				login(request, user)
				return redirect("/runstatus/")
			else:
				return redirect("/login/")
		else:
			return redirect("/login/")
	t = loader.get_template('login.html')
	c = RequestContext(request, {
		'login':True,
		})
	return HttpResponse(t.render(c))

def signout(request):
	if not request.user.is_active:
		return redirect("/login/")
	logout(request)
	return redirect("/login/")
