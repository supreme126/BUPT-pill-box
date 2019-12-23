Page({
  data: {
    index0: 0,

    clock: [{
      clock: wx.getStorageSync('clock1'),
    },
    {
      clock: wx.getStorageSync('clock2')
    },
    {
      clock: wx.getStorageSync('clock3')
    },
    {
      clock: wx.getStorageSync('clock4')
    },
    {
      clock: wx.getStorageSync('clock5')
    },
    {
      clock: wx.getStorageSync('clock6')
    },
    {
      clock: wx.getStorageSync('clock7')
    }
    ],

    clock_number_key: [{
      key: 'clock1',
    },
    {
      key: 'clock2'
    },
    {
      key: 'clock3'
    },
    {
      key: 'clock4'
    },
    {
      key: 'clock5'
    },
    {
      key: 'clock6'
    },
    {
      key: 'clock7'
    }
    ],
    radioItems: [{
      name: '上午',
      value: 'am',
      checked: ''
    },
    {
      name: '下午',
      value: 'pm',
      checked: ''
    }
    ],

    am_or_pm_data: '',
    time_data: '',
    kind_data: '',
    number_data: '',

    clock1: [{
      am_or_pm: '',
      time: '',
      kind: '',
      number: '',
    }]
  },

  onLoad: function () {

    var index_ = this.data.index0;
    index_ = wx.getStorageSync('index_edit')
    console.log("要编辑的闹钟编号：" + index_)
    this.setData({
      index0: index_,
    })//要编辑的闹钟编号

    var clock_ = this.data.clock;
    clock_[0] = wx.getStorageSync('clock1'),
      clock_[1] = wx.getStorageSync('clock2'),
      clock_[2] = wx.getStorageSync('clock3'),
      clock_[3] = wx.getStorageSync('clock4'),
      clock_[4] = wx.getStorageSync('clock5'),
      clock_[5] = wx.getStorageSync('clock6'),
      clock_[6] = wx.getStorageSync('clock7'),
      this.setData({
        clock: clock_,
      })//更新所有数据，找到要编辑的初始值

    var temporary_radio = this.data.radioItems;
    if (clock_[this.data.index0 - 1][0].am_or_pm == "am") {
      temporary_radio[0].checked = 'true';
      console.log("初始为上午被选中")
    }
    if (clock_[this.data.index0 - 1][0].am_or_pm == "pm") {
      temporary_radio[1].checked = 'true';
      console.log("初始为上午被选中")
    }
    this.setData({
      radioItems: temporary_radio,
    })//设置初始上下午
    console.log("初始时间为：" + clock_[this.data.index0 - 1][0].time);
    console.log("初始小药盒所在位置为：" + clock_[this.data.index0 - 1][0].kind);
    console.log("初始吃药量为：" + clock_[this.data.index0 - 1][0].number);
  },
  radioChange_am_or_pm: function (e) {
    if (e.detail.value.length == 1) {
      this.setData({
        am_or_pm_data: '0' + e.detail.value
      });
    } else {
      this.setData({
        am_or_pm_data: e.detail.value
      });
    }
    console.log("输入的上下午为：" + this.data.am_or_pm_data)
  },
  timeinput: function (e) {
    if (e.detail.value.length == 1) {
      this.setData({
        time_data: '0' + e.detail.value
      });
    } else {
      this.setData({
        time_data: e.detail.value
      });
    }
    console.log("输入的吃药时间为：" + this.data.time_data + "点")
  },
  kindinput: function (e) {
    if (e.detail.value.length == 1) {
      this.setData({
        kind_data: '0' + e.detail.value
      });
    } else {
      this.setData({
        kind_data: e.detail.value
      });
    }
    console.log("输入的小药盒所在位置为：" + this.data.kind_data)
  },
  numberinput: function (e) {
    if (e.detail.value.length == 1) {
      this.setData({
        number_data: '0' + e.detail.value
      });
    } else {
      this.setData({
        number_data: e.detail.value
      });
    }
    console.log("输入的吃药量为：" + this.data.number_data)
  },
  setStorageSync: function () {
    if (this.data.time_data == 0 || this.data.kind_data == 0 || this.data.number_data == 0) {
      wx.showToast({
        title: '输入内容不能为空',
        icon: 'none'
      })
    }
    else if (this.data.time_data.length > 2 || this.data.kind_data.length > 2 || this.data.number_data.length > 2) {
      wx.showToast({
        title: '输入内容不符合格式',
        icon: 'none'
      })
    }
    else {
      wx.showToast({
        title: '保存成功',
        icon: 'none'
      })
      var obj = {
        am_or_pm: this.data.am_or_pm_data,
        time: this.data.time_data,
        kind: this.data.kind_data,
        number: this.data.number_data,
      }
      this.data.clock1 = this.data.clock1.concat(obj);

      let clock_number_key = this.data.clock_number_key;
      var clock1 = this.data.clock1;
      clock1.splice(0, 1);////记录编辑后的闹钟的信息
      console.log("当次修改后的闹钟信息：" + this.data.clock1[0].am_or_pm + " " + this.data.clock1[0].time + " " + this.data.clock1[0].kind + " " + this.data.clock1[0].number);
      wx.setStorageSync(clock_number_key[this.data.index0 - 1].key, this.data.clock1);//把当次的clock存进数据库

      wx.redirectTo({
        url: '../checkclock/checkclock'
      })
    }
  }
})