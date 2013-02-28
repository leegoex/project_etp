from django.conf.urls import patterns, include, url

# Uncomment the next two lines to enable the admin:
from django.contrib import admin
admin.autodiscover()

urlpatterns = patterns('',
    # Examples:
    # url(r'^$', 'etpconfig.views.home', name='home'),
    # url(r'^etpconfig/', include('etpconfig.foo.urls')),

    # Uncomment the admin/doc line below to enable admin documentation:
    # url(r'^admin/doc/', include('django.contrib.admindocs.urls')),

    # Uncomment the next line to enable the admin:
    # url(r'^admin/', include(admin.site.urls)),
    url(r'^admin/', include(admin.site.urls)),
    url(r'^$', 'etpapp.views.signin'),
    url(r'^runstatus/$', 'etpapp.views.runstatus'),
    url(r'^basicsettings/$', 'etpapp.views.basicsettings'),
    url(r'^syssettings/$', 'etpapp.views.syssettings'),
    url(r'^log/$', 'etpapp.views.log'),
    url(r'^login/$', 'etpapp.views.signin'),
    url(r'^logout/$', 'etpapp.views.signout'),
)
