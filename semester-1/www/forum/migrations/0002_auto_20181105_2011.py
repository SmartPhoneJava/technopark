# Generated by Django 2.1.3 on 2018-11-05 17:11

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('forum', '0001_initial'),
    ]

    operations = [
        migrations.AddField(
            model_name='profile',
            name='views',
            field=models.IntegerField(default=0),
        ),
        migrations.AddField(
            model_name='question',
            name='views',
            field=models.IntegerField(default=0),
        ),
    ]
